// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/Classes/Sound/SoundBase.h"
#include "Engine/Classes/Particles/ParticleSystemComponent.h"
#include "Env_explosion.generated.h"

UCLASS()
class ENVIRONMENTS_API AEnv_explosion : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnv_explosion();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
		void Explode();
	

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Explosion)
		USoundBase* ExplosionSound;

	UPROPERTY(EditDefaultsOnly, BluePrintReadWrite, Category = Explosion)
		UParticleSystem* ExplosionParticle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=Explosion)
	float Duration;

private:


	
	float NextExplodeTime;

	uint32 bCanExplode : 1;
};
