// Fill out your copyright notice in the Description page of Project Settings.

#include "../Public/FPCharacter.h"
#include "Engine.h"

// Sets default values
AFPCharacter::AFPCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PlayerCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("PlayerCamera"));

	PlayerCamera->SetupAttachment(RootComponent);

	PlayerCamera->RelativeLocation = FVector(-39.f, 0.f, 64.f);

	Flashlight = CreateDefaultSubobject<USpotLightComponent>(TEXT("Flashlight"));

	Flashlight->SetupAttachment(PlayerCamera);

	Flashlight->RelativeLocation = FVector(0.f, 0.f,4.f);

	Flashlight->SetVisibility(false);

	WorldMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("3rd Person Mesh"));

	WorldMesh->SetupAttachment(RootComponent);

	WorldMesh->RelativeLocation = FVector(0.f, 0.f, -90.f);

	WorldMesh->RelativeRotation = FRotator(0.f, 0.f, -90.f);

	RunSpeed = 300.f;

	FlySprintSpeed = 1000.f;
	
	FlySpeed = 600.f;

	SprintSpeed = 600.f;

	GetCharacterMovement()->MaxFlySpeed = FlySpeed;

	GetCharacterMovement()->MaxWalkSpeed = RunSpeed;

	bIsFlashlightOn = false;

	RunStepRate = .6f;

	SprintStepRate = .3f;

	DefaultLean = 0.f;
	
	TimeBetweenSteps = RunStepRate;

	bIsZoomed = false;

	LeanAmount = .001f;

	LeanSpeed = 20.f;

	ZoomSpeed = 20.f;

	bUseLean = false;
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

	if (bUseLean == true)
	{
		UpdateCameraLean(DeltaTime);
	}
	if (!bIsMovingForward() && bIsSprinting)
	{
		StopSprint();
	}
}


// Called to bind functionality to input
void AFPCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	PlayerInputComponent->BindAxis("MoveForward", this, &AFPCharacter::MoveForward);

	PlayerInputComponent->BindAxis("MoveRight", this, &AFPCharacter::MoveRight);

	PlayerInputComponent->BindAxis("MoveUp", this, &AFPCharacter::MoveUp);

	PlayerInputComponent->BindAction("Fly", IE_Pressed, this, &AFPCharacter::OnFly);

	PlayerInputComponent->BindAxis("Turn", this, &AFPCharacter::Turn);
	
	PlayerInputComponent->BindAxis("LookUp", this, &AFPCharacter::LookUp);

	PlayerInputComponent->BindAction("Flashlight", IE_Pressed, this, &AFPCharacter::ToggleFlashlight);
	
	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &AFPCharacter::StartSprint);
	
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &AFPCharacter::StopSprint);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AFPCharacter::StartJump);
	
	PlayerInputComponent->BindAction("Quit", IE_Pressed, this, &AFPCharacter::OnQuit);

	PlayerInputComponent->BindAction("Zoom", IE_Pressed, this, &AFPCharacter::StartZoom);
	
	PlayerInputComponent->BindAction("Zoom", IE_Released, this, &AFPCharacter::StopZoom);

	PlayerInputComponent->BindAction("Use", IE_Pressed, this, &AFPCharacter::OnUse);
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

void AFPCharacter::MoveUp(float Scale)
{
	if (GetCharacterMovement()->IsFlying())
	{
		AddMovementInput(GetActorUpVector(), Scale);
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

void AFPCharacter::Turn(float Scale)
{
	AddControllerYawInput(Scale);
}

void AFPCharacter::LookUp(float Scale)
{
	AddControllerPitchInput(Scale);

	PlayerCamera->RelativeRotation.Pitch = GetController()->GetControlRotation().Pitch;
}

void AFPCharacter::OnLanded(const FHitResult & Hit)
{
	PlayFootstepSound(Hit);
}

void AFPCharacter::StartSprint()
{
	if (bIsMovingForward())
	{
		GetCharacterMovement()->MaxFlySpeed = FlySprintSpeed;
		GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;

		TimeBetweenSteps = SprintStepRate;

		bIsSprinting = true;
	}
}

void AFPCharacter::StopSprint()
{
	GetCharacterMovement()->MaxFlySpeed = FlySpeed;
	GetCharacterMovement()->MaxFlySpeed = FlySpeed;
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
			TargetLean = LeanAmount;
		}
		if (bIsMovingLeft)
		{
			float LeftLeanAmount = LeanAmount * -1.f;

			TargetLean = LeftLeanAmount;
		}
		DefaultLean = FMath::FInterpTo(DefaultLean, TargetLean, DeltaTime, LeanSpeed);
				
		PlayerCamera->RelativeRotation.Roll = DefaultLean;
	}
}

void AFPCharacter::OnUse()
{

}

void AFPCharacter::OnFly()
{
	if (GetCharacterMovement()->IsFlying())
	{
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	}
	else
	{
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
	}
}

bool AFPCharacter::bIsMovingForward()
{
	FVector PlayerVelocity = GetVelocity();
	FVector ForwardVector = GetActorForwardVector();
	float DotProduct = FVector::DotProduct(PlayerVelocity, ForwardVector);
	if (DotProduct > .7f)
	{
		return true;
	}
	return false;
}

FHitResult AFPCharacter::ForwardTrace()
{
	FHitResult RV_ForwardHit;

	static FName NAME_ForwardTrace(TEXT("Forward Trace"));

	FCollisionQueryParams QueryParams(NAME_ForwardTrace, false, this);

	QueryParams.TraceTag = NAME_ForwardTrace;

	const FVector TraceStart = PlayerCamera->GetComponentLocation();

	const FVector TraceEnd = TraceStart + GetControlRotation().Vector() * 256;

	GetWorld()->LineTraceSingleByChannel(RV_ForwardHit, TraceStart, TraceEnd, ECC_Visibility, QueryParams);
	
	if (RV_ForwardHit.Actor != NULL)
	{
		GEngine->AddOnScreenDebugMessage(1, 0.5f, FColor::Red, "HAI");
	}

	return RV_ForwardHit;
}