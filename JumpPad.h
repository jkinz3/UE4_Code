// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "JumpPad.generated.h"

UCLASS()
class ENVIRONMENTS_API AJumpPad : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AJumpPad();

	USceneComponent* SceneRoot;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = JumpPad)
		UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = JumpPad)
		class UBoxComponent* TriggerBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (MakeEditWidget = ""))
		FVector JumpTarget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = JumpPad, meta = (ClampMin = "0.1"))
		float JumpTime;

	UFUNCTION(BlueprintCallable, Category = JumpPad)
		FVector CalculateJumpVelocity(AActor* JumpActor);
	
	UFUNCTION()
		virtual void TriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	TArray<AActor*> PendingJumpActors;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	void Launch(AActor* Actor);
	
};
