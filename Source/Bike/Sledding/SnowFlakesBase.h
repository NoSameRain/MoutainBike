// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SnowFlakesBase.generated.h"

UCLASS()
class BIKE_API ASnowFlakesBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASnowFlakesBase();

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float maxRange = 1000.0f;				// max range to which this actor will be moved

	UStaticMeshComponent* PlayerMesh;		// reference to the player pawn mesh

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
