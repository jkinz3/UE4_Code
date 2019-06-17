// Fill out your copyright notice in the Description page of Project Settings.

#include "../Public/FPPlayerController.h"

void AFirstPersonController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAxis("MoveForward", this, &AFirstPersonController::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &AFirstPersonController::MoveRight);
	InputComponent->BindAxis("Turn", this, &AFirstPersonController::Turn);
	InputComponent->BindAxis("LookUp", this, &AFirstPersonController::LookUp);
	InputComponent->BindAxis("TurnRate", this, &AFirstPersonController::TurnRate);
	InputComponent->BindAxis("LookUpRate", this, &AFirstPersonController::LookUpRate);

	InputComponent->BindAction("Sprint", IE_Pressed, this, &AFirstPersonController::StartSprint);
	InputComponent->BindAction("Sprint", IE_Released, this, &AFirstPersonController::StopSprint);

	InputComponent->BindAction("Fire", IE_Pressed, this, &AFirstPersonController::OnFire);
	InputComponent->BindAction("Jump", IE_Pressed, this, &AFirstPersonController::OnJump);
	InputComponent->BindAction("Quit", IE_Pressed, this, &AFirstPersonController::OnQuit);
	InputComponent->BindAction("Use", IE_Pressed, this, &AFirstPersonController::OnUse);

	InputComponent->BindAction("Flashlight", IE_Pressed, this, &AFirstPersonController::ToggleFlashlight);
}

void AFirstPersonController::SetPawn(APawn * InPawn)
{
	AController::SetPawn(InPawn);
	FPCharacter = Cast<AFPCharacter>(InPawn);
}

void AFirstPersonController::MoveForward(float Scale)
{
	if (FPCharacter != nullptr)
	{
		FPCharacter->MoveForward(Scale);
	}
}

void AFirstPersonController::MoveRight(float Scale)
{
	if (FPCharacter != nullptr)
	{
		FPCharacter->MoveRight(Scale);
	}
}

void AFirstPersonController::Turn(float Scale)
{
	if (FPCharacter != nullptr)
	{
		FPCharacter->Turn(Scale);
	}
}

void AFirstPersonController::LookUp(float Scale)
{
	if (FPCharacter != nullptr)
	{
		FPCharacter->LookUp(Scale);
	}
}

void AFirstPersonController::TurnRate(float Scale)
{
	if (FPCharacter != nullptr)
	{
		FPCharacter->TurnAtRate(Scale);
	}
}

void AFirstPersonController::LookUpRate(float Scale)
{
	if (FPCharacter != nullptr)
	{
		FPCharacter->LookUpAtRate(Scale);
	}
}

void AFirstPersonController::StartSprint()
{
	if (FPCharacter != nullptr)
	{
		FPCharacter->StartSprint();
	}
}

void AFirstPersonController::StopSprint()
{
	if (FPCharacter != nullptr)
	{
		FPCharacter->StopSprint();
	}
}

void AFirstPersonController::OnQuit()
{
	if (FPCharacter != nullptr)
	{
		FPCharacter->OnQuit();
	}
}

void AFirstPersonController::OnUse()
{
	if (FPCharacter != nullptr)
	{
		FPCharacter->Use();
	}
}

void AFirstPersonController::OnFire()
{
	if (FPCharacter != nullptr)
	{
		FPCharacter->Fire();
	}
}

void AFirstPersonController::OnJump()
{
	if (FPCharacter != nullptr)
	{
		FPCharacter->OnJump();
	}
}

void AFirstPersonController::ToggleFlashlight()
{
	if (FPCharacter != nullptr)
	{
		FPCharacter->ToggleFlashlight();
	}
}
