// Fill out your copyright notice in the Description page of Project Settings.


#include "BicycleHUD.h"

// Fill out your copyright notice in the Description page of Project Settings.

#include "Blueprint/UserWidget.h"

void ABicycleHUD::DrawHUD() {


}

void ABicycleHUD::BeginPlay() {
	Super::BeginPlay();

	//GetWorldTimerManager().SetTimer(countdownTimerHandle, this, &ABicycleHUD::timerFinished, 3.0f, false);
	//ShowTimeWidget();
}

void ABicycleHUD::ShowGameWidget() {
	APlayerController* playerController = GetOwningPlayerController();
	if (playerController && GameWidgetClass) {
		GameWidget = CreateWidget<UUserWidget>(GetWorld(), GameWidgetClass);
		GameWidget->AddToViewport();
	}
	EnableInput(GetOwningPlayerController());
	UE_LOG(LogTemp, Warning, TEXT("Show Game"));
}

void ABicycleHUD::ShowTimeWidget() {
	APlayerController* playerController = GetOwningPlayerController();
	if (playerController && TimeWidgetClass) {
		TimeWidget = CreateWidget<UUserWidget>(GetWorld(), TimeWidgetClass);
		TimeWidget->AddToViewport();
	}
	UE_LOG(LogTemp, Warning, TEXT("Show Timer"));

	GetWorldTimerManager().SetTimer(countdownTimerHandle, this, &ABicycleHUD::timerFinished, 3.0f, false);
}

void ABicycleHUD::ShowFinishWidget() {
	APlayerController* playerController = GetOwningPlayerController();
	if (playerController && FinishWidgetClass) {
		FinishWidget = CreateWidget<UUserWidget>(GetWorld(), FinishWidgetClass);
		FinishWidget->AddToViewport();
	}
	UE_LOG(LogTemp, Warning, TEXT("Show Finish"));
}

void ABicycleHUD::ShowNotFinishWidget() {
	APlayerController* playerController = GetOwningPlayerController();
	if (playerController && NotFinishWidgetClass) {
		NotFinishWidget = CreateWidget<UUserWidget>(GetWorld(), NotFinishWidgetClass);
		NotFinishWidget->AddToViewport();
	}
	UE_LOG(LogTemp, Warning, TEXT("Show Not Finish"));
}

void ABicycleHUD::timerFinished() {
	ShowGameWidget();
	if (TimeWidget) {
		TimeWidget->RemoveFromViewport();
		TimeWidget = nullptr;
	}
	ShowGameWidget();
	UE_LOG(LogTemp, Warning, TEXT("Timer finished!"));
}


float ABicycleHUD::GetPlayerSpeed() {
	return Speed;
}