// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/Sound/SoundWave.h"
#include "Runtime/Engine/Classes/GameFramework/CharacterMovementComponent.h"
#include "Runtime/Engine/Classes/PhysicalMaterials/PhysicalMaterial.h"
#include "Runtime/Engine/Classes/Components/SpotLightComponent.h"
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

	void MoveForward(float Scale);

	void MoveRight(float Scale);

	void MoveUp(float Scale); 

	void StartJump();

	void Turn(float Scale);

	void LookUp(float Scale);

	virtual void Landed(const FHitResult& Hit);

	void StartSprint();

	void StopSprint();

	void ToggleFlashlight();

	void OnQuit();

	void HandleFootsteps();

	void StartZoom();

	void StopZoom();

	void UpdateCameraLean(float DeltaTime);

	void OnUse();

	void OnFly();

	void OnDeath();
	
	UFUNCTION(BlueprintCallable)
		bool IsSprinting();

	UFUNCTION(BlueprintCallable)
	virtual float TakeDamage(float Damage);

	FHitResult ForwardTrace();

	USoundBase* GetFootstepSound(EPhysicalSurface Surface, bool bIsJumping);

	void PlayFootstepSound(const FHitResult& DownHit, bool bIsJumping);

	FHitResult HandleFootstepTrace();

	uint32 bIsZoomed : 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Lean)
		float ZoomSpeed;

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Gameplay)
		float RunSpeed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Gameplay)
		float SprintSpeed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Gameplay)
		float FlySpeed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Gameplay)
		float FlySprintSpeed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Footsteps)
		float RunStepRate;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Footsteps)
		float SprintStepRate;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Footsteps)
		TMap < TEnumAsByte<EPhysicalSurface>, USoundBase*> WalkStepsMap;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Footsteps)
		TMap < TEnumAsByte<EPhysicalSurface>, USoundBase*> SprintStepsMap;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Footsteps)
		TMap <TEnumAsByte<EPhysicalSurface>, USoundBase*> LandStepsMap;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Gameplay)
		UCameraComponent* PlayerCamera;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Gameplay)
		USpotLightComponent* Flashlight;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Footsteps)
		USoundBase* DefaultStepSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Lean, meta=(ClampMin=".001", EditCondition = "bUseLean"))
		float LeanAmount;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Lean, meta=(EditCondition = "bUseLean"))
		float LeanSpeed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Gameplay)
		USoundWave* FlashlightSwitchSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Lean)
		uint32 bUseLean : 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Health)
		float MaxHealth;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Health)
		USoundBase* DamageSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Health)
		USoundBase* DeathSound;

private:

	uint32 bIsMovingRight : 1;

	uint32 bIsMovingLeft : 1;

	uint32 bIsFlashlightOn : 1;

	float TimeBetweenSteps;

	float LastStepTime;

	float NextStepTime;

	float TargetLean;

	float DefaultLean;

	float CurrentHealth;

	bool bIsSprinting : 1;
};
