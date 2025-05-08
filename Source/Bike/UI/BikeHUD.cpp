// Fill out your copyright notice in the Description page of Project Settings.


#include "BikeHUD.h"
#include "Blueprint/UserWidget.h"

void ABikeHUD::DrawHUD() {


}

void ABikeHUD::BeginPlay() {
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(countdownTimerHandle, this, &ABikeHUD::timerFinished, 3.0f, false);
	ShowTimeWidget();
}

void ABikeHUD::ShowGameWidget() {
	APlayerController *playerController= GetOwningPlayerController();
	if (playerController && GameWidgetClass) {
		GameWidget = CreateWidget<UUserWidget>(GetWorld(), GameWidgetClass);
		GameWidget->AddToViewport();
		UE_LOG(LogTemp, Warning, TEXT("ACCC Show Game"));
	}
	UE_LOG(LogTemp, Warning, TEXT("Show Game"));
}

void ABikeHUD::ShowTimeWidget() {
	APlayerController* playerController = GetOwningPlayerController();
	if (playerController && TimeWidgetClass) {
		TimeWidget = CreateWidget<UUserWidget>(GetWorld(), TimeWidgetClass);
		TimeWidget->AddToViewport();
	}
	UE_LOG(LogTemp, Warning, TEXT("Show Timer"));
}

void ABikeHUD::timerFinished() {
	ShowGameWidget();
	if (TimeWidget) {
		TimeWidget->RemoveFromViewport();
		TimeWidget = nullptr;
	}

	UE_LOG(LogTemp, Warning, TEXT("Timer finished!"));
}


