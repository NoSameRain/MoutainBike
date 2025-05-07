// Fill out your copyright notice in the Description page of Project Settings.


#include "BikePlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "BikePawn.h"

void ABikePlayerController::BeginPlay()
{
	Super::BeginPlay();
}

void ABikePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// get the enhanced input subsystem
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		// add the mapping context so we get controls
		Subsystem->AddMappingContext(InputMappingContext, 0);
	}
}

void ABikePlayerController::Tick(float Delta)
{
	Super::Tick(Delta);

	if (IsValid(VehiclePawn) )
	{
		//VehicleUI->UpdateSpeed(VehiclePawn->GetChaosVehicleMovement()->GetForwardSpeed());
		//VehicleUI->UpdateGear(VehiclePawn->GetChaosVehicleMovement()->GetCurrentGear());
	}
}

void ABikePlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	// get a pointer to the controlled pawn
	VehiclePawn = CastChecked<APawn>(InPawn);
}