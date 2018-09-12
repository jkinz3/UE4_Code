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
	AFPCharacter* MyPawn = Cast<AFPCharacter>(GetParentActor());

	if (MyPawn)
	{
		const float TargetFOV = MyPawn->bIsZoomed ? TargetingFOV : NormalFOV;
		DefaultFOV = FMath::FInterpTo(DefaultFOV, TargetFOV, DeltaTime, 20.0f);

		
	}
	GEngine->AddOnScreenDebugMessage(1, 0.5f, FColor::Red, "HAI");
	Super::UpdateCamera(DeltaTime);


}
