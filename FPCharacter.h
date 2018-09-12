// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Runtime/Engine/Classes/Camera/CameraComponent.h"
#include "FPCharacter.generated.h"

UCLASS()
class ENVIRONMENTS_API AFPCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFPCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	  
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void MoveForward(float Value);

	void MoveRight(float Value);

	void OnJump();

	virtual void OnLanded(FHitResult &Hit);
	
	void StartSprint();

	void StopSprint();

	void StartZoom();

	void StopZoom();


	uint32 bIsZoomed : 1;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		float WalkSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		float SprintSpeed;

	



	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
		float ZoomSpeed;



	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Camera)
		UCameraComponent* PlayerCamera;


private:

	FString DebugText;



};
