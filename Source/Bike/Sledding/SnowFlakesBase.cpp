// Fill out your copyright notice in the Description page of Project Settings.

#include "SnowFlakesBase.h"
#include <Kismet/GameplayStatics.h>

// Sets default values
ASnowFlakesBase::ASnowFlakesBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ASnowFlakesBase::BeginPlay()
{
	Super::BeginPlay();

	// get player static mesh component and set reference
	APawn* player = UGameplayStatics::GetPlayerPawn(this, 0);
	PlayerMesh = player->GetComponentByClass<UStaticMeshComponent>();
}

// Called every frame
void ASnowFlakesBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (PlayerMesh)
	{
		// get player pawn speed and direction
		FVector pawnLocation = PlayerMesh->GetComponentLocation();
		FVector pawnDirection = PlayerMesh->GetComponentVelocity();

		// limit range to max range
		float range = FMath::Clamp(pawnDirection.Length(), 0, maxRange);
		pawnDirection.Normalize();

		// set actor position according to player pawn position and velocity
		SetActorLocation(pawnLocation + pawnDirection * range);
	}
}

