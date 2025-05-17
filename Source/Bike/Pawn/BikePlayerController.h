// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BikePlayerController.generated.h"

/**
 * 
 */
UCLASS()
class BIKE_API ABikePlayerController : public APlayerController
{
	GENERATED_BODY()


protected:

	/** Input Mapping Context to be used for player input */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	class UInputMappingContext* InputMappingContext;

	/** Pointer to the controlled vehicle pawn */
	TObjectPtr<class APawn> VehiclePawn;

	// Begin Actor interface
protected:

	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

public:

	virtual void Tick(float Delta) override;

	// End Actor interface

	// Begin PlayerController interface

	UFUNCTION(BlueprintCallable, Category = "Speed")
	void StartScene();

	void ShowGameWidget();
protected:

	virtual void OnPossess(APawn* InPawn) override;

	// End PlayerController interface
	
private:
	class ABicycleHUD* BikeHUD;
};
