// Fill out your copyright notice in the Description page of Project Settings.


#include "SledSound.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundWave.h"
#include "Serialization/StructuredArchive.h"
#include <Kismet/GameplayStatics.h>

// Sets default values for this component's properties
USledSound::USledSound()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void USledSound::BeginPlay()
{
	Super::BeginPlay();

	RockHitTime = GetWorld()->GetRealTimeSeconds();
	TreeHitTime = GetWorld()->GetRealTimeSeconds();
	// ...

}


// Called every frame
void USledSound::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void USledSound::InitSoundPlayer(UAudioComponent* rockHitAudioComponent, UAudioComponent* treeHitAudioComponent,
	UAudioComponent* windAudioComponent, UAudioComponent* slideAudioComponent)
{
	RockHitAudioComponent = rockHitAudioComponent;
	TreeHitAudioComponent = treeHitAudioComponent;
	WindAudioComponent = windAudioComponent;
	SlideAudioComponent = slideAudioComponent;

	if (WindAudioComponent)
		WindAudioComponent->FadeIn(0.5f);

	if (SlideAudioComponent)
		SlideAudioComponent->FadeIn(0.5f);
}

void USledSound::UpdateSounds(float speed, bool grounded)
{
	//float newPitch = FMath::Clamp(0.f, 1.f, speed / SoundPitchScaleMaxSpeed) * PitchMultiplierRange;
	//newPitch = FMath::Clamp(newPitch, 1.f, newPitch);

	float newPitch = PitchMinimumThreshold + FMath::Clamp(speed / SoundPitchScaleMaxSpeed, 0.f, 1.f) * PitchMultiplierRange;

	if (WindAudioComponent)
		WindAudioComponent->SetPitchMultiplier(newPitch);

	if (SlideAudioComponent)
	{
		SlideAudioComponent->SetPitchMultiplier(newPitch);

		if (grounded && !SlideAudioComponent->IsPlaying() && speed > 100.f)
			SlideAudioComponent->FadeIn(0.5f, 1.0f);
		if (!grounded && SlideAudioComponent->IsPlaying())
			SlideAudioComponent->FadeOut(0.5f, 0.2f);
	}
}

void USledSound::PlayHitSounds(TArray<FName> tags, float speed)
{
	if (speed < HitSoundMinSpeed)
		return;

	float time = GetWorld()->GetRealTimeSeconds();

	if (time > RockHitTime + HitSoundInterval && tags.Contains("Rock") && RockHitAudioComponent)
	{
		RockHitAudioComponent->Play();
		RockHitTime = time;
	}
	else if (time > TreeHitTime + HitSoundInterval && tags.Contains("Tree") && TreeHitAudioComponent)
	{
		TreeHitAudioComponent->Play();
		TreeHitTime = time;
	}
}

