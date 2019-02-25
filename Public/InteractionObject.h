// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Camera/CameraComponent.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "InteractionObject.generated.h"

UCLASS()
class ENVIRONMENTS_API AInteractionObject : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInteractionObject();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		UStaticMeshComponent* MyMesh;

	UPROPERTY(EditAnywhere)
		USceneComponent* HoldingComponent;



	UFUNCTION()
		void PickUp(AFPCharacter* HoldingCharacter);

private:

	uint32 bHolding : 1;

	uint32 bGravity : 1;

	FRotator ControlRotation;

	AFPCharacter* MyCharacter;

	UCameraComponent* PlayerCamera;

	FVector ForwardVector;
	
};
