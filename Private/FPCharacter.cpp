// Fill out your copyright notice in the Description page of Project Settings.

#include "../Public/FPCharacter.h"
#include "Engine/Engine.h"
#include "Engine/StaticMesh.h"
#include "DrawDebugHelpers.h"
#include "Engine/Classes/GameFramework/CharacterMovementComponent.h"

//creating a print definition, because I hate typing this  out every time.
#define print(text) if (GEngine) GEngine->AddOnScreenDebugMessage(1, 1.5f, FColor::Blue, text)
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

	Flashlight->RelativeLocation = FVector(0.f, 0.f, 4.f);

	Flashlight->SetVisibility(false);

	HeldLocation = CreateDefaultSubobject<USceneComponent>(TEXT("Held Location"));
	HeldLocation->SetupAttachment(PlayerCamera);

	// Set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;
	WorldMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("3rd Person Mesh"));

	WorldMesh->SetupAttachment(RootComponent);

	WorldMesh->RelativeLocation = FVector(0.f, 0.f, -90.f);

	WorldMesh->RelativeRotation = FRotator(0.f, 0.f, -90.f);

	bEnableSprint = false;

	RunSpeed = 300.f;

	FlySprintSpeed = 1000.f;

	FlySpeed = 600.f;

	SprintSpeed = 600.f;

	bIsFlashlightOn = false;

	RunStepRate = .6f;

	SprintStepRate = .3f;

	DefaultLean = 0.f;

	LeanAmount = .001f;

	LeanSpeed = 20.f;

	bUseLean = false;

	CurrentHealth = MaxHealth;

	bIsHoldingObject = false;

	ThrowForce = 100000.f;
}

// Called when the game starts or when spawned
void AFPCharacter::BeginPlay()
{
	Super::BeginPlay();

	GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
	GetCharacterMovement()->MaxFlySpeed = FlySpeed;
	TimeBetweenSteps = RunStepRate;

}

// Called every frame
void AFPCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsHoldingObject && HoldingHandle != nullptr)
	{
		HoldingHandle->SetTargetLocation(HeldLocation->GetComponentLocation());
		FRotator NewRotation = FRotator::ZeroRotator;
		NewRotation.Yaw = GetActorRotation().Yaw;


		HeldActor->SetActorRotation(NewRotation);
	}
}

// Called to bind functionality to input
void AFPCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AFPCharacter::MoveForward(float Scale)
{
	if (Scale == 0.f)
	{
		return;
	}
	AddMovementInput(GetActorForwardVector(), Scale);

	HandleFootsteps();
}

void AFPCharacter::MoveRight(float Scale)
{
	if (Scale == 0.f)
	{
		return;
	}
	AddMovementInput(GetActorRightVector(), Scale);

	HandleFootsteps();

}

void AFPCharacter::Turn(float Scale)
{
	if (Scale == 0.f)
	{
		return;
	}
	AddControllerYawInput(Scale);

}

void AFPCharacter::LookUp(float Scale)
{
	if (Scale == 0.f)
	{
		return;
	}
	AddControllerPitchInput(Scale);
	PlayerCamera->RelativeRotation.Pitch = GetController()->GetControlRotation().Pitch;
}

void AFPCharacter::TurnAtRate(float Scale)
{
	if (Scale == 0.f)
	{
		return;
	}
	AddControllerYawInput(Scale* BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AFPCharacter::LookUpAtRate(float Scale)
{

	if (Scale == 0.f)
	{
		return;
	}
	AddControllerPitchInput(Scale * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AFPCharacter::OnJump()
{
	if (GetCharacterMovement()->IsMovingOnGround())
	{
		Jump();
		FHitResult DownHit = HandleFootstepTrace();

		PlayFootstepSound(DownHit, true);
	}
}

void AFPCharacter::Landed(const FHitResult & Hit)
{
	Super::Landed(Hit);
	PlayFootstepSound(Hit, false);


}

void AFPCharacter::ToggleFlashlight()
{
	if (bIsFlashlightOn)
	{
		Flashlight->SetVisibility(false);

		bIsFlashlightOn = false;

		if (FlashlightSwitchSound != nullptr)
		{
			UGameplayStatics::SpawnSoundAttached(FlashlightSwitchSound, GetRootComponent());
		}
	}
	else
	{
		Flashlight->SetVisibility(true);

		bIsFlashlightOn = true;
		if (FlashlightSwitchSound != nullptr)
		{
			UGameplayStatics::SpawnSoundAttached(FlashlightSwitchSound, GetRootComponent());
		}
	}
}

void AFPCharacter::Use()
{
	FHitResult UseHit = ForwardTrace();

	if (UseHit.Actor != NULL && UseHit.Component != NULL)
	{
		PickUpObject(UseHit.Actor.Get(), UseHit.Component.Get());

	}
}

void AFPCharacter::StartSprint()
{
	if (IsMovingForward() && IsSprintEnabled())
	{
		GetCharacterMovement()->MaxFlySpeed = FlySprintSpeed;
		GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;

		TimeBetweenSteps = SprintStepRate;

		bIsSprinting = true;
	}
}

void AFPCharacter::StopSprint()
{
	if (IsSprintEnabled())
	{
		GetCharacterMovement()->MaxFlySpeed = FlySpeed;

		GetCharacterMovement()->MaxWalkSpeed = RunSpeed;

		TimeBetweenSteps = RunStepRate;

		bIsSprinting = false;
	}
}

void AFPCharacter::Fire()
{
	if (bIsHoldingObject)
	{
		HoldingHandle->ReleaseComponent();
		bIsHoldingObject = false;
		UPrimitiveComponent* MeshToThrow = Cast<UPrimitiveComponent>(HeldActor->GetRootComponent());
		if (MeshToThrow != nullptr)
		{
			FVector ThrowVector = GetControlRotation().Vector() * ThrowForce;
			MeshToThrow->AddImpulse(ThrowVector);
		}
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

		PlayFootstepSound(FootstepHitResult, false);

		float CurrentTime = UGameplayStatics::GetRealTimeSeconds(GetWorld());

		LastStepTime = CurrentTime;

		NextStepTime = CurrentTime + TimeBetweenSteps;
	}
}

bool AFPCharacter::IsSprinting()
{
	return bIsSprinting;
}

bool AFPCharacter::IsSprintEnabled()
{
	return bEnableSprint;
}

bool AFPCharacter::IsMovingForward()
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
	const FVector TraceEnd = TraceStart + GetControlRotation().Vector() * 1000.f;

	GetWorld()->LineTraceSingleByChannel(RV_ForwardHit, TraceStart, TraceEnd, GetCapsuleComponent()->GetCollisionObjectType(), QueryParams);
	return RV_ForwardHit;

}

USoundBase * AFPCharacter::GetFootstepSound(EPhysicalSurface Surface, bool bIsJumping)
{
	USoundBase** SoundPtr = nullptr;

	if (IsSprintEnabled())
	{

		if (IsSprinting())
		{
			SoundPtr = SprintStepsMap.Find(Surface);
		}
		else if (!IsSprinting())
		{
			SoundPtr = WalkStepsMap.Find(Surface);
		}
		if (bIsJumping)
		{
			SoundPtr = SprintStepsMap.Find(Surface);
		}
		if (!bIsJumping && GetCharacterMovement()->IsFalling())
		{
			SoundPtr = LandStepsMap.Find(Surface);
		}
	}
	else
	{
		if (!bIsJumping && GetCharacterMovement()->IsFalling())
		{
			SoundPtr = LandStepsMap.Find(Surface);
		}
		SoundPtr = SprintStepsMap.Find(Surface);
	}
	return SoundPtr ? *SoundPtr : nullptr;
}

void AFPCharacter::PlayFootstepSound(const FHitResult & DownHit, bool bIsJumping)
{
	if (DownHit.PhysMaterial != NULL)
	{
		EPhysicalSurface SurfaceType = UPhysicalMaterial::DetermineSurfaceType(DownHit.PhysMaterial.Get());

		USoundBase* NewFootstepSound = GetFootstepSound(SurfaceType, bIsJumping);

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

void AFPCharacter::PickUpObject(AActor * ActorToPickup, UPrimitiveComponent* ActorComponent)
{
	if (!bIsHoldingObject)
	{
		UPhysicsHandleComponent* PhysicsHandleComponent = NewObject<UPhysicsHandleComponent>(ActorToPickup);
		PhysicsHandleComponent->RegisterComponent();

		HoldingHandle = PhysicsHandleComponent;

		PhysicsHandleComponent->GrabComponentAtLocation(ActorComponent, TEXT("None"), ActorToPickup->GetActorLocation());

		bIsHoldingObject = true;

		HeldActor = ActorToPickup;
	}
	else
	{
		if (HoldingHandle != nullptr)
		{
			HoldingHandle->ReleaseComponent();

			bIsHoldingObject = false;
		}
	}
}
