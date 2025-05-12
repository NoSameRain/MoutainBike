// Fill out your copyright notice in the Description page of Project Settings.


#include "BikePawn.h"

#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "ChaosWheeledVehicleMovementComponent.h"

#include "BikePlayerController.h"
#include "Bike/BikeAnimInstance.h"
#include "Animation/AnimComposite.h"
//#include "Runtime/LevelSequence/Public/LevelSequencePlayer.h"

// Sets default values
ABikePawn::ABikePawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	MainBicycleMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MainBicycleMesh"));
	SetRootComponent(MainBicycleMesh);
	
	BicycleMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("BicycleMesh"));
	BicycleMesh->SetupAttachment(RootComponent);

	HumanMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("HumanMesh"));
	HumanMesh->SetupAttachment(BicycleMesh);

	// construct the front camera boom
	FrontSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Front Spring Arm"));
	FrontSpringArm->SetupAttachment(RootComponent);
	FrontSpringArm->TargetArmLength = 0.0f;
	FrontSpringArm->bDoCollisionTest = false;
	FrontSpringArm->bEnableCameraRotationLag = true;
	FrontSpringArm->CameraRotationLagSpeed = 15.0f;
	FrontSpringArm->SetRelativeLocation(FVector(30.0f, 0.0f, 120.0f));

	FrontCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Front Camera"));
	FrontCamera->SetupAttachment(FrontSpringArm);
	FrontCamera->bAutoActivate = false;

	// construct the back camera boom
	BackSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Back Spring Arm"));
	BackSpringArm->SetupAttachment(RootComponent);
	BackSpringArm->TargetArmLength = 650.0f;
	BackSpringArm->SocketOffset.Z = 150.0f;
	BackSpringArm->bDoCollisionTest = false;
	BackSpringArm->bInheritPitch = false;
	BackSpringArm->bInheritRoll = false;
	BackSpringArm->bEnableCameraRotationLag = true;
	BackSpringArm->CameraRotationLagSpeed = 2.0f;
	BackSpringArm->CameraLagMaxDistance = 50.0f;

	BackCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Back Camera"));
	BackCamera->SetupAttachment(BackSpringArm);

	MainBicycleMesh->SetSimulatePhysics(true);
	MainBicycleMesh->SetCollisionProfileName(TEXT("Vehicle"));

	// Create the vehicle movement component
	ChaosVehicleMovement = CreateDefaultSubobject<UChaosWheeledVehicleMovementComponent>(TEXT("VehicleMovement"));

	ChaosVehicleMovement->ChassisHeight = 144.0f;
	ChaosVehicleMovement->DragCoefficient = 0.31f;

	//Set up the wheels
	ChaosVehicleMovement->bLegacyWheelFrictionPosition = true;
	ChaosVehicleMovement->WheelSetups.SetNum(2);

	// NOTE: Check the Blueprint asset for the Torque Curve
	ChaosVehicleMovement->EngineSetup.MaxTorque = 750.0f;
	ChaosVehicleMovement->EngineSetup.MaxRPM = 7000.0f;
	ChaosVehicleMovement->EngineSetup.EngineIdleRPM = 900.0f;
	ChaosVehicleMovement->EngineSetup.EngineBrakeEffect = 0.2f;
	ChaosVehicleMovement->EngineSetup.EngineRevUpMOI = 5.0f;
	ChaosVehicleMovement->EngineSetup.EngineRevDownRate = 600.0f;

	// Set up the transmission
	ChaosVehicleMovement->TransmissionSetup.bUseAutomaticGears = true;
	ChaosVehicleMovement->TransmissionSetup.bUseAutoReverse = true;
	ChaosVehicleMovement->TransmissionSetup.FinalRatio = 2.81f;
	ChaosVehicleMovement->TransmissionSetup.ChangeUpRPM = 6000.0f;
	ChaosVehicleMovement->TransmissionSetup.ChangeDownRPM = 2000.0f;
	ChaosVehicleMovement->TransmissionSetup.GearChangeTime = 0.2f;
	ChaosVehicleMovement->TransmissionSetup.TransmissionEfficiency = 0.9f;

	ChaosVehicleMovement->TransmissionSetup.ForwardGearRatios.SetNum(5);
	ChaosVehicleMovement->TransmissionSetup.ForwardGearRatios[0] = 4.25f;
	ChaosVehicleMovement->TransmissionSetup.ForwardGearRatios[1] = 2.52f;
	ChaosVehicleMovement->TransmissionSetup.ForwardGearRatios[2] = 1.66f;
	ChaosVehicleMovement->TransmissionSetup.ForwardGearRatios[3] = 1.22f;
	ChaosVehicleMovement->TransmissionSetup.ForwardGearRatios[4] = 1.0f;

	ChaosVehicleMovement->TransmissionSetup.ReverseGearRatios.SetNum(1);
	ChaosVehicleMovement->TransmissionSetup.ReverseGearRatios[0] = 4.04f;

	// Set up the steering
	// NOTE: Check the Blueprint asset for the Steering Curve
	ChaosVehicleMovement->SteeringSetup.SteeringType = ESteeringType::Ackermann;
	ChaosVehicleMovement->SteeringSetup.AngleRatio = 0.7f;
}

// Called when the game starts or when spawned
void ABikePawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called to bind functionality to input
void ABikePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// steering 
		EnhancedInputComponent->BindAction(SteeringAction, ETriggerEvent::Triggered, this, &ABikePawn::Steering);
		EnhancedInputComponent->BindAction(SteeringAction, ETriggerEvent::Completed, this, &ABikePawn::Steering);

		// throttle 
		EnhancedInputComponent->BindAction(ThrottleAction, ETriggerEvent::Triggered, this, &ABikePawn::Throttle);
		EnhancedInputComponent->BindAction(ThrottleAction, ETriggerEvent::Completed, this, &ABikePawn::Throttle);

		// break 
		EnhancedInputComponent->BindAction(BrakeAction, ETriggerEvent::Triggered, this, &ABikePawn::Brake);
		EnhancedInputComponent->BindAction(BrakeAction, ETriggerEvent::Started, this, &ABikePawn::StartBrake);
		EnhancedInputComponent->BindAction(BrakeAction, ETriggerEvent::Completed, this, &ABikePawn::StopBrake);

		// handbrake 
		EnhancedInputComponent->BindAction(HandbrakeAction, ETriggerEvent::Started, this, &ABikePawn::StartHandbrake);
		EnhancedInputComponent->BindAction(HandbrakeAction, ETriggerEvent::Completed, this, &ABikePawn::StopHandbrake);

		// handbrake 
		EnhancedInputComponent->BindAction(WheelieAction, ETriggerEvent::Started, this, &ABikePawn::StartWheelie);
		EnhancedInputComponent->BindAction(WheelieAction, ETriggerEvent::Completed, this, &ABikePawn::StopWheelie);

		// look around 
		EnhancedInputComponent->BindAction(LookAroundAction, ETriggerEvent::Triggered, this, &ABikePawn::LookAround);

		// toggle camera 
		EnhancedInputComponent->BindAction(ToggleCameraAction, ETriggerEvent::Triggered, this, &ABikePawn::ToggleCamera);

		// reset the vehicle 
		EnhancedInputComponent->BindAction(ResetVehicleAction, ETriggerEvent::Triggered, this, &ABikePawn::ResetVehicle);

		EnhancedInputComponent->BindAction(RespawnAction, ETriggerEvent::Triggered, this, &ABikePawn::Respawn);
	}
}

void ABikePawn::Tick(float Delta)
{
	Super::Tick(Delta);

	// add some angular damping if the vehicle is in midair
	bool bMovingOnGround = ChaosVehicleMovement->IsMovingOnGround();
	MainBicycleMesh->SetAngularDamping(bMovingOnGround ? 0.0f : 3.0f);

	// realign the camera yaw to face front
	float CameraYaw = BackSpringArm->GetRelativeRotation().Yaw;
	CameraYaw = FMath::FInterpTo(CameraYaw, 0.0f, Delta, 1.0f);

	BackSpringArm->SetRelativeRotation(FRotator(0.0f, CameraYaw, 0.0f));
}

void ABikePawn::Steering(const FInputActionValue& Value)
{
	// get the input magnitude for steering
	float SteeringValue = Value.Get<float>();

	float speed = ChaosVehicleMovement->GetForwardSpeed();

	float steerCoefficient = speed > 50.f ? 0.01 : 0.1f;

	if (SteeringValue == 0.0f) {
		// reset the steering value
		additiveSteeringValue = 0.f;
	}
	else {
		// add the input to the steering value
		additiveSteeringValue += SteeringValue * steerCoefficient;

	}

	// add the input
	ChaosVehicleMovement->SetSteeringInput(additiveSteeringValue);
}

void ABikePawn::Throttle(const FInputActionValue& Value)
{
	// get the input magnitude for the throttle
	float ThrottleValue = Value.Get<float>();

	// add the input
	ChaosVehicleMovement->SetThrottleInput(ThrottleValue);
}

void ABikePawn::Brake(const FInputActionValue& Value)
{
	// get the input magnitude for the brakes
	float BreakValue = Value.Get<float>();

	// add the input
	ChaosVehicleMovement->SetBrakeInput(BreakValue);
}

void ABikePawn::StartBrake(const FInputActionValue& Value)
{
	// call the Blueprint hook for the break lights
	BrakeLights(true);
}

void ABikePawn::StopBrake(const FInputActionValue& Value)
{
	// call the Blueprint hook for the break lights
	BrakeLights(false);

	// reset brake input to zero
	ChaosVehicleMovement->SetBrakeInput(0.0f);
}

void ABikePawn::StartHandbrake(const FInputActionValue& Value)
{
	// add the input
	ChaosVehicleMovement->SetHandbrakeInput(true);

	// call the Blueprint hook for the break lights
	BrakeLights(true);
	UE_LOG(LogTemp, Error, TEXT("Handbrake Vehicle"));
}

void ABikePawn::StopHandbrake(const FInputActionValue& Value)
{
	// add the input
	ChaosVehicleMovement->SetHandbrakeInput(false);

	// call the Blueprint hook for the break lights
	BrakeLights(false);
}

void ABikePawn::LookAround(const FInputActionValue& Value)
{
	// get the flat angle value for the input 
	float LookValue = Value.Get<float>();

	// add the input
	BackSpringArm->AddLocalRotation(FRotator(0.0f, LookValue, 0.0f));
}

void ABikePawn::ToggleCamera(const FInputActionValue& Value)
{
	// toggle the active camera flag
	bFrontCameraActive = !bFrontCameraActive;

	FrontCamera->SetActive(bFrontCameraActive);
	BackCamera->SetActive(!bFrontCameraActive);
}

void ABikePawn::ResetVehicle(const FInputActionValue& Value)
{
	// reset to a location slightly above our current one
	FVector ResetLocation = GetActorLocation() + FVector(0.0f, 0.0f, 50.0f);

	// reset to our yaw. Ignore pitch and roll
	FRotator ResetRotation = GetActorRotation();
	ResetRotation.Pitch = 0.0f;
	ResetRotation.Roll = 0.0f;

	// teleport the actor to the reset spot and reset physics
	SetActorTransform(FTransform(ResetRotation, ResetLocation, FVector::OneVector), false, nullptr, ETeleportType::TeleportPhysics);

	MainBicycleMesh->SetPhysicsAngularVelocityInDegrees(FVector::ZeroVector);
	MainBicycleMesh->SetPhysicsLinearVelocity(FVector::ZeroVector);

	UE_LOG(LogTemp, Error, TEXT("Reset Vehicle"));
}

void ABikePawn::Respawn(const FInputActionValue& Value) {
	if (reachedCheckPoint) {
		FVector test = GetActorLocation();
		SetActorTransform(FTransform(LastCheckPointRotation, LastCheckPointLocation, FVector::OneVector), false, nullptr, ETeleportType::TeleportPhysics);
		MainBicycleMesh->SetPhysicsAngularVelocityInDegrees(FVector::ZeroVector);
		MainBicycleMesh->SetPhysicsLinearVelocity(FVector::ZeroVector);
		UE_LOG(LogTemp, Error, TEXT("Respawn Vehicle at %s vs actors prev: %s"), *LastCheckPointLocation.ToString() , *test.ToString());
	}

	
}

void ABikePawn::StartWheelie(const FInputActionValue& Value) {
	bIsWheelie = true;
}

void ABikePawn::StopWheelie(const FInputActionValue& Value) {
	bIsWheelie = false;
}


void ABikePawn::VictorySceneAndAnimation() {
	// play the animation montage
	ABikePlayerController* PlayerController = Cast<ABikePlayerController>(GetController());
	if (PlayerController) {
		DisableInput(PlayerController);
		MainBicycleMesh->SetPhysicsAngularVelocityInDegrees(FVector::ZeroVector);
		MainBicycleMesh->SetPhysicsLinearVelocity(FVector::ZeroVector);
	}
	UBikeAnimInstance* animInst = Cast<UBikeAnimInstance>(HumanMesh->GetAnimInstance());
	if (animInst) {
		animInst->hasWon = true;
	}

}