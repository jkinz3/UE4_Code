// Fill out your copyright notice in the Description page of Project Settings.

#include "JumpPad.h"
#include "FPCharacter.h"
#include "Runtime/Engine/Classes/Components/BoxComponent.h"
#include "Runtime/Engine/Classes/Components/CapsuleComponent.h"
#include "Runtime/Engine/Classes/GameFramework/CharacterMovementComponent.h"
#include "Runtime/Engine/Classes/Components/StaticMeshComponent.h"


// Sets default values
AJumpPad::AJumpPad()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	RootComponent = SceneRoot;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	Mesh->SetupAttachment(RootComponent);

	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("no"));
	TriggerBox->SetCollisionProfileName(TEXT("Trigger"));
	TriggerBox->SetupAttachment(RootComponent);
	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &AJumpPad::TriggerBeginOverlap);

	JumpTarget = FVector(100.0f, 0.0f, 0.0f);
	JumpTime = 1.0f;


}

FVector AJumpPad::CalculateJumpVelocity(AActor * JumpActor)
{
	FVector Target = ActorToWorld().TransformPosition(JumpTarget) - JumpActor->GetActorLocation();
	const float GravityZ = GetWorld()->GetGravityZ();
	Target.Z += GetDefault<AFPCharacter>()->GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight();
	
	float SizeZ = Target.Z / JumpTime + 0.5f * -GravityZ * JumpTime;
	float SizeXY = Target.Size2D() / JumpTime;

	FVector Velocity = Target.GetSafeNormal2D() * SizeXY + FVector(0.0f, 0.0f, SizeZ);

	ACharacter* Char = Cast<ACharacter>(JumpActor);
	if (Char != NULL && Char->GetCharacterMovement() != NULL && Char->GetCharacterMovement()->GravityScale != 1.0f)
	{
		Velocity *= FMath::Sqrt(Char->GetCharacterMovement()->GravityScale);
	}
	return Velocity;

	return FVector();
}

void AJumpPad::TriggerBeginOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (!PendingJumpActors.Contains(OtherActor))
	{
		PendingJumpActors.Add(OtherActor);
	}
}

// Called when the game starts or when spawned
void AJumpPad::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AJumpPad::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (PendingJumpActors.Num() > 0)
	{
		for (AActor* Actor : PendingJumpActors)
		{
			Launch(Actor);
		}
		PendingJumpActors.Reset();
	}

}

void AJumpPad::Launch(AActor * Actor)
{
	ACharacter* Char = Cast<ACharacter>(Actor);
	if (Char)
	{
		Char->LaunchCharacter(CalculateJumpVelocity(Char), true, true);
	}
} 
