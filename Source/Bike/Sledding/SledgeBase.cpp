// Fill out your copyright notice in the Description page of Project Settings.


#include "SledgeBase.h"

// Sets default values
ASledgeBase::ASledgeBase()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void ASledgeBase::ApplySteerTorque()
{
	FVector Torque = SledgeMesh->GetUpVector() * sTorque * yaw;

	SledgeMesh->AddTorqueInRadians(Torque);
}

void ASledgeBase::ApplyForwardForce()
{
	FVector Force = SledgeMesh->GetForwardVector() * LForce;
	if (isGrounded)
	{
		SledgeMesh->AddForce(Force);
	}
}

void ASledgeBase::ApplyAirControl()
{
	FVector Torque = (SledgeMesh->GetForwardVector() * sTorque) * roll;

	SledgeMesh->AddTorqueInRadians(Torque);

	Torque = (SledgeMesh->GetRightVector() * sTorque) * pitch;

	SledgeMesh->AddTorqueInRadians(Torque);
}

void ASledgeBase::ApplyDirectionalFriction()
{
	velocity = SledgeMesh->GetComponentVelocity();
	if (velocity.IsNearlyZero()) return;

	const FVector rightVector = SledgeMesh->GetRightVector();
	const FVector sideVel = FVector::DotProduct(velocity, rightVector) * rightVector;

	const FVector sideFriction = -sideVel;

	float surfaceFriction = matFriction;

	if (!isGrounded)
		surfaceFriction = 0.3f;

	surfaceFriction *= 5.0f;
	FVector finalFriction = sideFriction * surfaceFriction;

	SledgeMesh->AddForce(finalFriction, NAME_None, true);
}

void ASledgeBase::ResetValues()
{
	yaw = 0.0f;
	pitch = 0.0f;
	roll = 0.0f;
	isGrounded = false;
}

void ASledgeBase::setStaticMesh(UStaticMeshComponent* mesh)
{
	SledgeMesh = mesh;
}

// Called when the game starts or when spawned
void ASledgeBase::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ASledgeBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}