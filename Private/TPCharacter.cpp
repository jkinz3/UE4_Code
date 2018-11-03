// Fill out your copyright notice in the Description page of Project Settings.

#include "TPCharacter.h"
#include "Runtime/Engine/Classes/Camera/CameraComponent.h"
#include "Engine.h"
#include "Runtime/Engine/Classes/GameFramework/SpringArmComponent.h"
#include "Runtime/Engine/Classes/GameFramework/CharacterMovementComponent.h"


// Sets default values
ATPCharacter::ATPCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	GetCharacterMovement()->JumpZVelocity = 300.f;
	GetCharacterMovement()->AirControl = 0.2f;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.f, 0.0f);
	
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("Camera Boom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f;
	CameraBoom->bUsePawnControlRotation = true;

	FlySpeed = 600.f;
	GetCharacterMovement()->MaxFlySpeed = FlySpeed;
	FlySprintSpeed = 2000.f;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Player Camera"));
	FollowCamera->SetupAttachment(CameraBoom);
	FollowCamera->bUsePawnControlRotation = false;
	ResetSpeed = 20.f;
	
}

// Called when the game starts or when spawned
void ATPCharacter::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ATPCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	float ActorPitch = GetActorRotation().Pitch;
	float DefaultPitch = ActorPitch;
	float ActorYaw = GetActorRotation().Yaw;
	float ActorRoll = GetActorRotation().Roll;

	GEngine->AddOnScreenDebugMessage(1, 3.f, FColor::White, FString::SanitizeFloat(DefaultPitch));

	if (!GetCharacterMovement()->IsFlying())
	{

		DefaultPitch = FMath::FInterpTo(DefaultPitch, 0.f, DeltaTime, ResetSpeed);
		FRotator NewRotation = FRotator(DefaultPitch, ActorYaw, ActorRoll);
	
	
	}
	else
	{
		
	}
	
	
}

// Called to bind functionality to input
void ATPCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	PlayerInputComponent->BindAxis("MoveForward", this, &ATPCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ATPCharacter::MoveRight);
	PlayerInputComponent->BindAxis("MoveUp", this, &ATPCharacter::MoveUp);
	PlayerInputComponent->BindAction("Sprint", IE_Pressed,  this, &ATPCharacter::StartFlySprint);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &ATPCharacter::StopFlySprint);

	PlayerInputComponent->BindAxis("LookUp", this, &ATPCharacter::LookUp);
	PlayerInputComponent->BindAxis("Turn", this, &ATPCharacter::Turn);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ATPCharacter::OnJump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ATPCharacter::StopJump);

	PlayerInputComponent->BindAction("Fly", IE_Pressed, this, &ATPCharacter::OnFly);
}

void ATPCharacter::MoveForward(float Scale)
{
	if ((Controller != NULL) && (Scale != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Scale);
	}
}

void ATPCharacter::MoveRight(float Scale)
{
	if ((Controller != NULL) && (Scale != 0.0f))
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Scale);
	}
}

void ATPCharacter::MoveUp(float Scale)
{
	if (GetCharacterMovement()->IsFlying())
	{
		AddMovementInput(GetActorUpVector(), Scale);
	}
}

void ATPCharacter::StartFlySprint()
{
	if(GetCharacterMovement()->IsFlying())
	{
		GetCharacterMovement()->MaxFlySpeed = FlySprintSpeed;
	}
}

void ATPCharacter::StopFlySprint()
{
	if (GetCharacterMovement()->IsFlying())
	{
		GetCharacterMovement()->MaxFlySpeed = FlySpeed;
		
	}
}

void ATPCharacter::OnFly()
{
	if (GetCharacterMovement()->IsFlying())
	{
		bUseControllerRotationPitch = false;

		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	}
	else
	{
		bUseControllerRotationPitch = true;
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
	}
}

void ATPCharacter::LookUp(float Scale)
{
	AddControllerPitchInput(Scale);
}

void ATPCharacter::Turn(float Scale)
{
	AddControllerYawInput(Scale);
}

void ATPCharacter::OnJump()
{
	Jump();
}

void ATPCharacter::StopJump()
{
	StopJumping();
}

void ATPCharacter::OnQuit()
{
	FGenericPlatformMisc::RequestExit(true);
}