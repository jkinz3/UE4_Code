  // Fill out your copyright notice in the Description page of Project Settings.

#include "../Public/FPCameraManager.h"
#include "../Public/FPCharacter.h"



AFPCameraManager::AFPCameraManager(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{	
	NormalFOV = 90.f;
	
	TargetingFOV = 60.0f;
	 
	ViewPitchMin = -87.f;
	
	ViewPitchMax = 87.0f;
	
	bAlwaysApplyModifiers = true;
}

void AFPCameraManager::UpdateCamera(float DeltaTime)
{
	AFPCharacter* MyPawn = Cast<AFPCharacter>(GetOwningPlayerController()->GetPawn());

	if (MyPawn)
	{
		float TargetFOV = 90.f;

	//	DefaultFOV = FMath::FInterpTo(DefaultFOV, TargetFOV, DeltaTime, MyPawn->ZoomSpeed);

		SetFOV(DefaultFOV);
	}
	Super::UpdateCamera(DeltaTime);
}
