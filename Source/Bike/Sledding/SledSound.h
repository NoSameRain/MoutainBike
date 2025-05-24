// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SledSound.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BIKE_API USledSound : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USledSound();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sounds")
	float HitSoundInterval = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sounds")
	float HitSoundMinSpeed = 30.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sounds")
	float PitchMinimumThreshold = 0.2f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sounds")
	float PitchMultiplierRange = 2.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sounds")
	float SoundPitchScaleMaxSpeed = 5000.f;

	UAudioComponent* RockHitAudioComponent;
	UAudioComponent* TreeHitAudioComponent;
	UAudioComponent* WindAudioComponent;
	UAudioComponent* SlideAudioComponent;

	float RockHitTime;
	float TreeHitTime;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "Sounds")
	void InitSoundPlayer(UAudioComponent* rockHitAudioComponent, UAudioComponent* treeHitAudioComponent,
		UAudioComponent* windAudioComponent, UAudioComponent* slideAudioComponent);

	UFUNCTION(BlueprintCallable, Category = "Sounds")
	void UpdateSounds(float speed, bool grounded);

	UFUNCTION(BlueprintCallable, Category = "Sounds")
	void PlayHitSounds(TArray<FName> tags, float speed);
};
