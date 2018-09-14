// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "FPCameraManager.h"
#include "FPPlayerController.generated.h"

/**
*
*/
UCLASS()
class ENVIRONMENTS_API AFPPlayerController : public APlayerController
{
	GENERATED_BODY()

	AFPPlayerController(const FObjectInitializer& ObjectInitializer);
};
