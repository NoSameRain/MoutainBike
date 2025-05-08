// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "BicycleHUD.generated.h"

/**
 * 
 */
UCLASS()
class BIKE_API ABicycleHUD : public AHUD
{
	GENERATED_BODY()

public:
	virtual void DrawHUD() override;

	UPROPERTY(EditAnywhere, Category = "PlayerStats")
	TSubclassOf<class UUserWidget> GameWidgetClass;

	class UUserWidget* GameWidget;

	void ShowGameWidget();

	UPROPERTY(EditAnywhere, Category = "Time")
	TSubclassOf<class UUserWidget> TimeWidgetClass;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite, Category = "Time")
	class UUserWidget* TimeWidget;

	void ShowTimeWidget();

	UPROPERTY(EditAnywhere, Category = "Speed")
	float Speed;

	UFUNCTION(BlueprintCallable, Category = "Speed")
	float GetPlayerSpeed();
	


protected:
	virtual void BeginPlay() override;

	FTimerHandle countdownTimerHandle;

	void timerFinished();

	
};
