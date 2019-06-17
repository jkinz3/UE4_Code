// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FPCharacter.h"
#include "GameFramework/PlayerController.h"
#include "FPPlayerController.generated.h"

/**
 *
 */
UCLASS()
class ENVIRONMENTS_API AFirstPersonController : public APlayerController
{
	GENERATED_BODY()

		virtual void SetupInputComponent() override;
	virtual void SetPawn(APawn* InPawn) override;

	void MoveForward(float Scale);
	void MoveRight(float Scale);
	void Turn(float Scale);
	void LookUp(float Scale);
	void TurnRate(float Scale);
	void LookUpRate(float Scale);
	void StartSprint();
	void StopSprint();

	void OnQuit();

	void OnUse();
	void OnFire();
	void OnJump();
	void ToggleFlashlight();


private:


	UPROPERTY()
		AFPCharacter* FPCharacter;
};
