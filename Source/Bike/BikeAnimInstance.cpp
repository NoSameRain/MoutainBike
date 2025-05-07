// Fill out your copyright notice in the Description page of Project Settings.


#include "BikeAnimInstance.h"
#include "ChaosWheeledVehicleMovementComponent.h"
#include "Pawn/BikePawn.h"

void UBikeAnimInstance::NativeInitializeAnimation() {
	Super::NativeInitializeAnimation();

	// get a pointer to the controlled pawn
	BikePawn = Cast<ABikePawn>(TryGetPawnOwner());

}

void UBikeAnimInstance::NativeUpdateAnimation(float DeltaTime) {
	Super::NativeUpdateAnimation(DeltaTime);

	if (BikePawn == nullptr) {
		BikePawn = Cast<ABikePawn>(TryGetPawnOwner());
	}

	if (BikePawn == nullptr) {
		return;
	}




	Speed = BikePawn->ChaosVehicleMovement->GetForwardSpeed();

	Lean = BikePawn->ChaosVehicleMovement->GetSteeringInput();

	isAccelerating = Speed > 5.0f;

	isWheelie = BikePawn->getIsWheelie();

	//VehiclePawn->GetChaosVehicleMovement()->GetCurrentGear()

	//isWheelie = BicyclePawn->GetChaosVehicleMovement()->IsWheelie();

	//isPedaling = BicyclePawn->GetChaosVehicleMovement()->IsPedaling();


}