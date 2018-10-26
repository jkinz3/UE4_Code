// Fill out your copyright notice in the Description page of Project Settings.

#include "TPCharacter.h"
#include "Runtime/Engine/Classes/Camera/CameraComponent.h"
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

	

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Player Camera"));
	FollowCamera->SetupAttachment(CameraBoom);
	FollowCamera->bUsePawnControlRotation = false;


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

}

// Called to bind functionality to input
void ATPCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	PlayerInputComponent->BindAxis("MoveForward", this, &ATPCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ATPCharacter::MoveRight);

	PlayerInputComponent->BindAxis("LookUp", this, &ATPCharacter::LookUp);
	PlayerInputComponent->BindAxis("Turn", this, &ATPCharacter::Turn);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ATPCharacter::OnJump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ATPCharacter::StopJump);
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