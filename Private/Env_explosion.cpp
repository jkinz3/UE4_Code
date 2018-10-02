// Fill out your copyright notice in the Description page of Project Settings.

#include "../Public/Env_explosion.h"
#include "Engine.h"



// Sets default values
AEnv_explosion::AEnv_explosion()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bCanExplode = true;

	NextExplodeTime = 0.0f;

	Duration = .5f;
}

// Called when the game starts or when spawned
void AEnv_explosion::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnv_explosion::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnv_explosion::Explode()
{
	float CurrentTime = UGameplayStatics::GetRealTimeSeconds(GetWorld());

	if (CurrentTime >= NextExplodeTime && ExplosionParticle && ExplosionSound)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionParticle,  GetActorLocation());

		NextExplodeTime = CurrentTime + Duration;

		UGameplayStatics::SpawnSoundAtLocation(this, ExplosionSound, GetActorLocation());
	}
}

