// Fill out your copyright notice in the Description page of Project Settings.

#include "FPCameraManager.h"
#include "FPCharacter.h"
#include "Engine.h"


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
		float TargetFOV;
		if (MyPawn->bIsZoomed)
		{
			TargetFOV = TargetingFOV;
		}
		else
		{
			TargetFOV = NormalFOV;
		}

		DefaultFOV = FMath::FInterpTo(DefaultFOV, TargetFOV, DeltaTime, 20.0f);


		SetFOV(DefaultFOV);

		GEngine->AddOnScreenDebugMessage(1, 0.5f, FColor::Red, FString::SanitizeFloat(DefaultFOV));
	
	
	}

	

	Super::UpdateCamera(DeltaTime);


}
