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

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite, Category = "Time")
	class UUserWidget* GameWidget;

	UFUNCTION(BlueprintCallable, Category = "Speed")
	void ShowGameWidget();

	UPROPERTY(EditAnywhere, Category = "Time")
	TSubclassOf<class UUserWidget> TimeWidgetClass;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite, Category = "Time")
	class UUserWidget* TimeWidget;

	UPROPERTY(EditAnywhere, Category = "Time")
	TSubclassOf<class UUserWidget> FinishWidgetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Time")
	class UUserWidget* FinishWidget;

	UPROPERTY(EditAnywhere, Category = "Time")
	TSubclassOf<class UUserWidget> NotFinishWidgetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Time")
	class UUserWidget* NotFinishWidget;

	UFUNCTION(BlueprintCallable, Category = "Speed")
	void ShowTimeWidget();

	UPROPERTY(EditAnywhere, Category = "Speed")
	float Speed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Completion")
	bool isCompleted;

	UFUNCTION(BlueprintCallable, Category = "Speed")
	float GetPlayerSpeed();
	

	UFUNCTION(BlueprintCallable, Category = "Speed")
	void timerFinished();
		
	UFUNCTION(BlueprintCallable, Category = "Speed")
	void ShowFinishWidget();
	
	UFUNCTION(BlueprintCallable, Category = "Speed")
	void ShowNotFinishWidget();

protected:
	virtual void BeginPlay() override;

	FTimerHandle countdownTimerHandle;


	
};
