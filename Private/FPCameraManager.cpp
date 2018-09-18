// Fill out your copyright notice in the Description page of Project Settings.

#include "FPCameraManager.h"
#include "FPCharacter.h"
#include "Engine.h"


AFPCameraManager::AFPCameraManager(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	SprintFOV = 95.f;
	
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
		if (MyPawn->bIsZoomed && !MyPawn->bIsSprinting)
		{
			TargetFOV = TargetingFOV;
		}
		if(!MyPawn->bIsZoomed && !MyPawn->bIsSprinting)
		{
			TargetFOV = NormalFOV;
		}
		if (!MyPawn->bIsZoomed && MyPawn->bIsSprinting)
		{
			TargetFOV = SprintFOV;
		}

		DefaultFOV = FMath::FInterpTo(DefaultFOV, TargetFOV, DeltaTime, MyPawn->ZoomSpeed);

		SetFOV(DefaultFOV);
	}

	GEngine->AddOnScreenDebugMessage(1, .5f, FColor::White, FString::SanitizeFloat(DefaultFOV));
	Super::UpdateCamera(DeltaTime);
}