// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "SledgeBase.generated.h"

UCLASS()
class BIKE_API ASledgeBase : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ASledgeBase();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SledgeBase")
	bool isGrounded;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SledgeBase")
	float yaw;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SledgeBase")
	float roll;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SledgeBase")
	float pitch;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SledgeBase")
	float sTorque = 500000.0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SledgeBase")
	float LForce = 500.0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SledgeBase")
	FVector velocity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SledgeBase")
	float speed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SledgeBase")
	UStaticMeshComponent* SledgeMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SledgeBase")
	float matFriction;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "SledgeBase")
	void ApplySteerTorque();

	UFUNCTION(BlueprintCallable, Category = "SledgeBase")
	void ApplyForwardForce();

	UFUNCTION(BlueprintCallable, Category = "SledgeBase")
	void ApplyAirControl();

	UFUNCTION(BlueprintCallable, Category = "SledgeBase")
	void ApplyDirectionalFriction();

	UFUNCTION(BlueprintCallable, Category = "SledgeBase")
	void ResetValues();

	UFUNCTION(BlueprintCallable, Category = "SledgeBase")
	void setStaticMesh(UStaticMeshComponent* mesh);

};
