// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TPCharacter.generated.h"

UCLASS()
class ENVIRONMENTS_API ATPCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ATPCharacter();

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

	void StartFlySprint();

	void StopFlySprint();

	void OnFly();
	
	void LookUp(float Scale);

	void Turn(float Scale);

	void OnJump();

	void StopJump();
	
	void OnQuit();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Camera)
		class USpringArmComponent* CameraBoom;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Camera)
		class UCameraComponent* FollowCamera;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Gameplay)
		float FlySpeed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Gameplay)
		float FlySprintSpeed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Gameplay)
		float ResetSpeed;

	float DefaultPitch;  

};
 