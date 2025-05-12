// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "BikeAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class BIKE_API UBikeAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaTime) override;

	UPROPERTY(BlueprintReadOnly, Category = "Bicycle", meta = (AllowPrivateAccess = "true"))
	bool hasWon;
private:
	UPROPERTY(BlueprintReadOnly, Category = "Bicycle", meta = (AllowPrivateAccess = "true"))
	class ABikePawn* BikePawn;

	UPROPERTY(BlueprintReadOnly, Category = "Bicycle", meta = (AllowPrivateAccess = "true"))
	float Speed;

	UPROPERTY(BlueprintReadOnly, Category = "Bicycle", meta = (AllowPrivateAccess = "true"))
	float Lean;

	UPROPERTY(BlueprintReadOnly, Category = "Bicycle", meta = (AllowPrivateAccess = "true"))
	bool isWheelie;

	UPROPERTY(BlueprintReadOnly, Category = "Bicycle", meta = (AllowPrivateAccess = "true"))
	bool isPedaling;


	UPROPERTY(BlueprintReadOnly, Category = "Bicycle", meta = (AllowPrivateAccess = "true"))
	bool isAccelerating;
	
};
