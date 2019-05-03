// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraShake.h"
#include "Sound/SoundWave.h"
#include "Runtime/Engine/Classes/Components/CapsuleComponent.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/GameFramework/CharacterMovementComponent.h"
#include "Runtime/Engine/Classes/PhysicalMaterials/PhysicalMaterial.h"
#include "Runtime/Engine/Classes/Components/SpotLightComponent.h"
#include "Runtime/Engine/Classes/Camera/CameraComponent.h"
#include "GameFramework/Actor.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
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
	void Turn(float Scale);
	void LookUp(float Scale);
	void TurnAtRate(float Scale);
	void LookUpAtRate(float Scale);
	void StartSprint();
	void StopSprint();

	void Fire();
	void OnQuit();
	void OnJump();
	virtual void Landed(const FHitResult& Hit) override;
	void ToggleFlashlight();
	void Use();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Gameplay)
		USceneComponent* HeldLocation;

protected:

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseLookUpRate;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Movement)
		float RunSpeed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Movement, meta = (EditCondition = "bEnableSprint"))
		float SprintSpeed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Movement)
		float FlySpeed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Movement)
		float FlySprintSpeed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Movement)
		float ThrowForce;

	/*camera shakes*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Movement)
		TSubclassOf<UCameraShake> WalkCameraShake;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Footsteps)
		float RunStepRate;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Footsteps, meta = (ClampMin = ".001", EditCondition = "bEnableSprint"))
		float SprintStepRate;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Footsteps)
		bool bEnableSprint;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Footsteps)
		TMap < TEnumAsByte<EPhysicalSurface>, USoundBase*> WalkStepsMap;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Footsteps, meta = (EditCondition = "bEnableSprint"))
		TMap < TEnumAsByte<EPhysicalSurface>, USoundBase*> SprintStepsMap;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Footsteps)
		TMap <TEnumAsByte<EPhysicalSurface>, USoundBase*> LandStepsMap;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Gameplay)
		UCameraComponent* PlayerCamera;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Gameplay)
		USpotLightComponent* Flashlight;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Mesh)
		USkeletalMeshComponent* WorldMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Footsteps)
		USoundBase* DefaultStepSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Lean, meta = (ClampMin = ".001", EditCondition = "bUseLean"))
		float LeanAmount;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Lean, meta = (EditCondition = "bUseLean"))
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

	void HandleFootsteps();

	UFUNCTION(BlueprintCallable)
		bool IsMovingForward();

	UFUNCTION(BlueprintCallable)
		bool IsSprinting();

	UFUNCTION(BlueprintCallable)
		bool IsSprintEnabled();

	FHitResult ForwardTrace();

	USoundBase* GetFootstepSound(EPhysicalSurface Surface, bool bIsJumping);

	void PlayFootstepSound(const FHitResult& DownHit, bool bIsJumping);

	void PlayCameraShake();

	FHitResult HandleFootstepTrace();

	void PickUpObject(AActor* ActorToPickup, UPrimitiveComponent* ActorComponent);

private:

	AActor* HeldActor;

	uint32 bIsMovingRight : 1;

	uint32 bIsMovingLeft : 1;

	uint32 bIsFlashlightOn : 1;

	uint32 bIsHoldingObject : 1;

	UPhysicsHandleComponent* HoldingHandle;

	float TimeBetweenSteps;

	float LastStepTime;

	float NextStepTime;

	float TargetLean;

	float DefaultLean;

	float CurrentHealth;

	bool bIsSprinting;


};
