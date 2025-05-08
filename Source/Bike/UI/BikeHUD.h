// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "BikeHUD.generated.h"

/**
 * 
 */
UCLASS()
class BIKE_API ABikeHUD : public AHUD
{
	GENERATED_BODY()

public:
	virtual void DrawHUD() override;

	UPROPERTY(EditAnywhere , Category = "PlayerStats")
	TSubclassOf<class UUserWidget> GameWidgetClass;

	class UUserWidget* GameWidget;

	void ShowGameWidget();

	UPROPERTY(EditAnywhere, Category = "Time")
	TSubclassOf<class UUserWidget> TimeWidgetClass;

	class UUserWidget* TimeWidget;

	void ShowTimeWidget();

protected:
	virtual void BeginPlay() override;

	FTimerHandle countdownTimerHandle;

	void timerFinished();

	UPROPERTY(EditAnywhere, Category = "Speed")
	float Speed;
};
