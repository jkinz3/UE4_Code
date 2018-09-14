// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/PlayerCameraManager.h"
#include "FPCameraManager.generated.h"

/**
*
*/
UCLASS()
class ENVIRONMENTS_API AFPCameraManager : public APlayerCameraManager
{
	GENERATED_BODY()

		AFPCameraManager(const FObjectInitializer& ObjectInitializer);


	/** normal FOV */
	float NormalFOV;

	/** targeting FOV */
	float TargetingFOV;

	/*sprinting FOV*/
	float SprintFOV;

	/** After updating camera, inform pawn to update 1p mesh to match camera's location&rotation */
	virtual void UpdateCamera(float DeltaTime) override;



};
