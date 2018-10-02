// Fill out your copyright notice in the Description page of Project Settings.

#include "../Public/FPPlayerController.h"
AFPPlayerController::AFPPlayerController(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PlayerCameraManagerClass = AFPCameraManager::StaticClass();
}

