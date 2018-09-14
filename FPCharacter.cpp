// Fill out your copyright notice in the Description page of Project Settings.

#include "FPCharacter.h"
#include "Engine.h"

// Sets default values
AFPCharacter::AFPCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PlayerCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("PlayerCamera"));

	PlayerCamera->SetupAttachment(RootComponent);

	PlayerCamera->RelativeLocation = FVector(-39.f, 0.f, 64.f);

	PlayerCamera->bUsePawnControlRotation = true;

	Flashlight = CreateDefaultSubobject<USpotLightComponent>(TEXT("Flashlight"));

	Flashlight->SetupAttachment(PlayerCamera);

	Flashlight->RelativeLocation = FVector(0.f, 0.f,4.f);

	Flashlight->SetVisibility(false);

	RunSpeed = 300.f;

	SprintSpeed = 600.f;

	GetCharacterMovement()->MaxWalkSpeed = RunSpeed;

	bIsFlashlightOn = false;

	RunStepRate = .6f;

	SprintStepRate = .3f;

	DefaultLean = 0.f;
	
	TimeBetweenSteps = RunStepRate;

	bIsZoomed = false;

	RightLean = 90.f;
	
	LeftLean = -90.f;

	LeanSpeed = 20.f;

	ZoomSpeed = 20.f;
}

// Called when the game starts or when spawned
void AFPCharacter::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AFPCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateCameraLean(DeltaTime);
}


// Called to bind functionality to input
void AFPCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	PlayerInputComponent->BindAxis("MoveForward", this, &AFPCharacter::MoveForward);

	PlayerInputComponent->BindAxis("MoveRight", this, &AFPCharacter::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &AFPCharacter::AddControllerYawInput);
	
	PlayerInputComponent->BindAxis("LookUp", this, &AFPCharacter::AddControllerPitchInput);

	PlayerInputComponent->BindAction("Flashlight", IE_Pressed, this, &AFPCharacter::ToggleFlashlight);
	
	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &AFPCharacter::StartSprint);
	
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &AFPCharacter::StopSprint);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AFPCharacter::StartJump);
	
	PlayerInputComponent->BindAction("Quit", IE_Pressed, this, &AFPCharacter::OnQuit);

	PlayerInputComponent->BindAction("Zoom", IE_Pressed, this, &AFPCharacter::StartZoom);
	
	PlayerInputComponent->BindAction("Zoom", IE_Released, this, &AFPCharacter::StopZoom);
}

void AFPCharacter::MoveForward(float Scale)
{
	if (Scale == 0)
	{
		return;
	}
	AddMovementInput(GetActorForwardVector(), Scale);
	
	HandleFootsteps();
}

void AFPCharacter::MoveRight(float Scale)
{
	if (Scale == 0)
	{
		bIsMovingLeft = false;

		bIsMovingRight = false;

		return;
	}
	
	AddMovementInput(GetActorRightVector(), Scale);
	
	HandleFootsteps();

	if (Scale == 1)
	{
		bIsMovingRight = true;
		
		bIsMovingLeft = false;
	}
	if (Scale == -1)
	{
		bIsMovingLeft = true;
		
		bIsMovingRight = false;
	}
}

void AFPCharacter::StartJump()
{
	if (GetCharacterMovement()->IsMovingOnGround())
	{
		Jump();
		
		FHitResult DownHit = HandleFootstepTrace();
		
		PlayFootstepSound(DownHit);
	}
}

void AFPCharacter::Landed(const FHitResult & Hit)
{
	PlayFootstepSound(Hit);
}

void AFPCharacter::StartSprint()
{
	GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
	
	TimeBetweenSteps = SprintStepRate;
	
	bIsSprinting = true;
}

void AFPCharacter::StopSprint()
{
	GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
	
	TimeBetweenSteps = RunStepRate;
	
	bIsSprinting = false;
}

void AFPCharacter::ToggleFlashlight()
{
	if (bIsFlashlightOn)
	{
		Flashlight->SetVisibility(false);
		
		bIsFlashlightOn = false;
		
		UGameplayStatics::SpawnSoundAttached(FlashlightSwitchSound, GetRootComponent());
	}
	else
	{
		Flashlight->SetVisibility(true);
	
		bIsFlashlightOn = true;
	
		UGameplayStatics::SpawnSoundAttached(FlashlightSwitchSound, GetRootComponent());
	}
}

void AFPCharacter::OnQuit()
{
	FGenericPlatformMisc::RequestExit(false);
}

void AFPCharacter::HandleFootsteps()
{
	if (UGameplayStatics::GetRealTimeSeconds(GetWorld()) >= NextStepTime && GetCharacterMovement()->IsMovingOnGround())
	{
		FHitResult FootstepHitResult = HandleFootstepTrace();
	
		PlayFootstepSound(FootstepHitResult);

		float CurrentTime = UGameplayStatics::GetRealTimeSeconds(GetWorld());

		LastStepTime = CurrentTime;

		NextStepTime = CurrentTime + TimeBetweenSteps;
	}
}

void AFPCharacter::StartZoom()
{
	bIsZoomed = true;
}

void AFPCharacter::StopZoom()
{
	bIsZoomed = false;
}

USoundBase* AFPCharacter::GetFootstepSound(EPhysicalSurface Surface)
{
	USoundBase** SoundPtr = nullptr;
	
	SoundPtr = FootstepSoundsMap.Find(Surface);

	return SoundPtr ? *SoundPtr : nullptr;
}

void AFPCharacter::PlayFootstepSound(const FHitResult& DownHit)
{
	if (DownHit.PhysMaterial != NULL)
	{
		EPhysicalSurface SurfaceType = UPhysicalMaterial::DetermineSurfaceType(DownHit.PhysMaterial.Get());

		USoundBase* NewFootstepSound = GetFootstepSound(SurfaceType);

		if (NewFootstepSound)
		{
			UGameplayStatics::SpawnSoundAtLocation(this, NewFootstepSound, DownHit.Location);
		}
		else
		{
			UGameplayStatics::SpawnSoundAtLocation(this, DefaultStepSound, DownHit.Location);
		}
	}
}

FHitResult AFPCharacter::HandleFootstepTrace()
{
	static FName NAME_FootstepTrace(TEXT("FootstepTrace"));

	FCollisionQueryParams QueryParams(NAME_FootstepTrace, false, this);

	QueryParams.bReturnPhysicalMaterial = true;

	float PawnRadius;

	float PawnHalfHeight;

	GetCapsuleComponent()->GetScaledCapsuleSize(PawnRadius, PawnHalfHeight);

	const FVector LineTraceStart = GetCapsuleComponent()->GetComponentLocation();

	const float TraceDistance = PawnHalfHeight + 10.0f;

	const FVector DownDirection = FVector(0.0f, 0.0f, (-TraceDistance - 200.0f));

	FHitResult RV_DownHit;

	GetWorld()->LineTraceSingleByChannel(RV_DownHit, LineTraceStart, LineTraceStart + DownDirection, GetCapsuleComponent()->GetCollisionObjectType(), QueryParams);
	
	return RV_DownHit;
}

void AFPCharacter::UpdateCameraLean(float DeltaTime)
{
	if (PlayerCamera)
	{
		if (!bIsMovingLeft && !bIsMovingRight)
		{
			TargetLean = 0.0f;
		}
		if (bIsMovingRight)
		{
			TargetLean = RightLean;
		}
		if (bIsMovingLeft)
		{
			TargetLean = LeftLean;
		}
		
		DefaultLean = FMath::FInterpTo(DefaultLean, TargetLean, DeltaTime, LeanSpeed);
		
		PlayerCamera->RelativeRotation.Roll = DefaultLean;
	}
}

