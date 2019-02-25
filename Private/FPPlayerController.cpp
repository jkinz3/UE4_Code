// Fill out your copyright notice in the Description page of Project Settings.

#include "../Public/FPPlayerController.h"

void AFPPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAxis("MoveForward", this, &AFPPlayerController::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &AFPPlayerController::MoveRight);
	InputComponent->BindAxis("Turn", this, &AFPPlayerController::Turn);
	InputComponent->BindAxis("LookUp", this, &AFPPlayerController::LookUp);
	InputComponent->BindAxis("TurnRate", this, &AFPPlayerController::TurnRate);
	InputComponent->BindAxis("LookUpRate", this, &AFPPlayerController::LookUpRate);

	InputComponent->BindAction("Sprint", IE_Pressed, this, &AFPPlayerController::StartSprint);
	InputComponent->BindAction("Sprint", IE_Released, this, &AFPPlayerController::StopSprint);

	InputComponent->BindAction("Fire", IE_Pressed, this, &AFPPlayerController::OnFire);
	InputComponent->BindAction("Jump", IE_Pressed, this, &AFPPlayerController::OnJump);
	InputComponent->BindAction("Quit", IE_Pressed, this, &AFPPlayerController::OnQuit);
	InputComponent->BindAction("Use", IE_Pressed, this, &AFPPlayerController::OnUse);

	InputComponent->BindAction("Flashlight", IE_Pressed, this, &AFPPlayerController::ToggleFlashlight);
}

void AFPPlayerController::SetPawn(APawn * InPawn)
{
	AController::SetPawn(InPawn);
	FPCharacter = Cast<AFPCharacter>(InPawn);
}

void AFPPlayerController::MoveForward(float Scale)
{
	if (FPCharacter != nullptr)
	{
		FPCharacter->MoveForward(Scale);
	}
}

void AFPPlayerController::MoveRight(float Scale)
{
	if (FPCharacter != nullptr)
	{
		FPCharacter->MoveRight(Scale);
	}
}

void AFPPlayerController::Turn(float Scale)
{
	if (FPCharacter != nullptr)
	{
		FPCharacter->Turn(Scale);
	}
}

void AFPPlayerController::LookUp(float Scale)
{
	if (FPCharacter != nullptr)
	{
		FPCharacter->LookUp(Scale);
	}
}

void AFPPlayerController::TurnRate(float Scale)
{
	if (FPCharacter != nullptr)
	{
		FPCharacter->TurnAtRate(Scale);
	}
}

void AFPPlayerController::LookUpRate(float Scale)
{
	if (FPCharacter != nullptr)
	{
		FPCharacter->LookUpAtRate(Scale);
	}
}

void AFPPlayerController::StartSprint()
{
	if (FPCharacter != nullptr)
	{
		FPCharacter->StartSprint();
	}
}

void AFPPlayerController::StopSprint()
{
	if (FPCharacter != nullptr)
	{
		FPCharacter->StopSprint();
	}
}

void AFPPlayerController::OnQuit()
{
	if (FPCharacter != nullptr)
	{
		FPCharacter->OnQuit();
	}
}

void AFPPlayerController::OnUse()
{
	if (FPCharacter != nullptr)
	{
		FPCharacter->Use();
	}
}

void AFPPlayerController::OnFire()
{
	if (FPCharacter != nullptr)
	{
		FPCharacter->Fire();
	}
}

void AFPPlayerController::OnJump()
{
	if (FPCharacter != nullptr)
	{
		FPCharacter->OnJump();
	}
}

void AFPPlayerController::ToggleFlashlight()
{
	if (FPCharacter != nullptr)
	{
		FPCharacter->ToggleFlashlight();
	}
}
