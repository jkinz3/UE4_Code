// Fill out your copyright notice in the Description page of Project Settings.

#include "FPCharacter.h"
#include "Engine.h"
#include "FPCameraManager.h"
#include "Runtime/Engine/Classes/GameFramework/CharacterMovementComponent.h"



// Sets default values
AFPCharacter::AFPCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	PlayerCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Player Camera"));
	PlayerCamera->SetupAttachment(RootComponent);
	PlayerCamera->SetRelativeLocation(FVector(0.0f, 0.0f, 90.f));

	WalkSpeed = 600;
	
	


	SprintSpeed = 1000;

	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;

	
	bIsZoomed = false;

	
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
	
	APlayerController* PC = Cast<APlayerController>(Controller);
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, PC->PlayerCameraManagerClass->GetFullName());
}

// Called to bind functionality to input
void AFPCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AFPCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AFPCharacter::MoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &AFPCharacter::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &AFPCharacter::AddControllerPitchInput);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AFPCharacter::OnJump);
	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &AFPCharacter::StartSprint);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &AFPCharacter::StopSprint);
	PlayerInputComponent->BindAction("Zoom", IE_Pressed, this, &AFPCharacter::StartZoom);
	PlayerInputComponent->BindAction("Zoom", IE_Released, this, &AFPCharacter::StopZoom);
}

void AFPCharacter::MoveForward(float Value)
{
	if (Value == 0)
	{
		return;
	}
	AddMovementInput(GetActorForwardVector(), Value);

}

void AFPCharacter::MoveRight(float Value)
{
	if (Value == 0)
	{
		return;
	}
	AddMovementInput(GetActorRightVector(), Value);
}

void AFPCharacter::OnJump()
{
	Jump();
}

void AFPCharacter::OnLanded(FHitResult & Hit)
{
}

void AFPCharacter::StartSprint()
{
	GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
}

void AFPCharacter::StopSprint()
{
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}

void AFPCharacter::StartZoom()
{ 
	bIsZoomed = true;
	
}

void AFPCharacter::StopZoom()
{
	bIsZoomed = false;

	
}


