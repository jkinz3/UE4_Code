// Fill out your copyright notice in the Description page of Project Settings.

#include "InteractionObject.h"
#include "FPCharacter.h"


// Sets default values
AInteractionObject::AInteractionObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	MyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("My Mesh"));
	RootComponent = MyMesh;

	bHolding = false;
	bGravity = true;

}

// Called when the game starts or when spawned
void AInteractionObject::BeginPlay()
{
	Super::BeginPlay();

	
}

// Called every frame
void AInteractionObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bHolding && MyCharacter != nullptr)
	{
		SetActorLocationAndRotation(MyCharacter->GetHoldingLocation(), FRotator(0,0,0));
	}

}

void AInteractionObject::PickUp(AFPCharacter* HoldingCharacter)
{
	bHolding = true;
	MyCharacter = HoldingCharacter;

}

