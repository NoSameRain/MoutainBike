// Fill out your copyright notice in the Description page of Project Settings.


#include "PathGenerator.h"
#include "Kismet/KismetRenderingLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "Landscape.h"
#include "LandscapeComponent.h"
#include "Materials/MaterialParameterCollection.h"
#include "Materials/MaterialParameterCollectionInstance.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "Engine/World.h"

// Sets default values
APathGenerator::APathGenerator()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SkierMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ball"));
	SetRootComponent(SkierMesh);

	SkierMesh->SetSimulatePhysics(true);
	SkierMesh->SetNotifyRigidBodyCollision(true);
	SkierMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	SkierMesh->SetCollisionProfileName(TEXT("BlockAll"));

	FootprintCapture = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("FootprintCapture"));
	FootprintCapture->SetupAttachment(RootComponent);
	FootprintCapture->ProjectionType = ECameraProjectionMode::Orthographic;
	FootprintCapture->OrthoWidth = 100.f;
	FootprintCapture->SetRelativeRotation(FRotator(90.f, 0.f, 0.f));
	FootprintCapture->SetRelativeLocation(FVector(0.f, 0.f, -100.f));
	FootprintCapture->TextureTarget = ImprintRenderTarget;

	TerrainCapture = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("TerrainCapture"));
	TerrainCapture->SetupAttachment(RootComponent);
	TerrainCapture->ProjectionType = ECameraProjectionMode::Orthographic;
	TerrainCapture->OrthoWidth = 100.f;
	TerrainCapture->SetRelativeRotation(FRotator(90.f, 0.f, 0.f));
	TerrainCapture->SetRelativeLocation(FVector(0.f, 0.f, -100.f));
	FootprintCapture->TextureTarget = TerrainRenderTarget;

	TerrainActor = nullptr;
}

// Called when the game starts or when spawned
void APathGenerator::BeginPlay()
{
	Super::BeginPlay();

	TrailBlenderShaderInstance = UMaterialInstanceDynamic::Create(TrailBlenderShader, this);

	UKismetRenderingLibrary::ClearRenderTarget2D(this, ImprintRenderTarget, FLinearColor(0.5f, 0.5f, 0.5f));
	UKismetRenderingLibrary::ClearRenderTarget2D(this, TrailRenderTarget, FLinearColor(0.5f, 0.5f, 0.5f));
	UKismetRenderingLibrary::ClearRenderTarget2D(this, TrailRenderTargetTemp, FLinearColor(0.5f, 0.5f, 0.5f));

	/*SkierMesh = GetWorld()->GetFirstPlayerController()->GetPawn()->GetComponentByClass<USkeletalMeshComponent>();
	SkierMesh->OnComponentHit.AddDynamic(this, &APathGenerator::OnHit);*/

	ACharacter* player = UGameplayStatics::GetPlayerCharacter(this, 0);
	SkierMesh = player->GetMesh();
	player->GetCapsuleComponent()->OnComponentHit.AddDynamic(this, &APathGenerator::OnHit);

	PreviousPosition = SkierMesh->Bounds.Origin;

	FMatrix rotationMatrix = FMatrix(FVector(0.f, 0.f, 1.f), FVector(1.f, 0.f, 0.f), FVector(0.f, 1.f, 0.f), FVector::ZeroVector);
	CaptureOrientation = rotationMatrix.Rotator();
	CaptureDistanceFromOrigin = FVector(0.f, 0.f, -300.f);

	ImprintRenderTarget->ClearColor = FLinearColor(0.5f, 0.5f, 0.5f);

	CaptureWidth = 1000.f;

	FMatrix OrthoMatrix = FReversedZOrthoMatrix(CaptureWidth / 2.0f, CaptureWidth / 2.0f, 1.0f / (600.f - 0.1f), -0.1f / (600.f - 0.1f));

	FootprintCapture->CustomProjectionMatrix = OrthoMatrix;
	FootprintCapture->bUseCustomProjectionMatrix = true;
	FootprintCapture->SetWorldRotation(FRotator(90.f, 0.f, 0.f));
	FootprintCapture->SetRelativeLocation(CaptureDistanceFromOrigin);
	FootprintCapture->TextureTarget = ImprintRenderTarget;
	FootprintCapture->ShowFlags.Atmosphere = false;
	//FootprintCapture->ShowFlags.Cloud = false;
	FootprintCapture->ShowFlags.Fog = false;
	FootprintCapture->ShowOnlyComponents.Empty();
	FootprintCapture->ShowOnlyComponents.Add(SkierMesh);

	TArray<USceneComponent*> ChildrenComponents;
	SkierMesh->GetChildrenComponents(true, ChildrenComponents);

	for (USceneComponent* Comp : ChildrenComponents) {
		FootprintCapture->ShowOnlyComponents.Add(Cast<UPrimitiveComponent>(Comp));
	}

	//FootprintCapture->ShowOnlyComponent(SkierMesh);
	FootprintCapture->CompositeMode = ESceneCaptureCompositeMode::SCCM_Overwrite;
	FootprintCapture->PrimitiveRenderMode = ESceneCapturePrimitiveRenderMode::PRM_UseShowOnlyList;
	FootprintCapture->CaptureSource = ESceneCaptureSource::SCS_SceneDepth;
	FootprintCapture->bCaptureEveryFrame = false;
	FootprintCapture->bCaptureOnMovement = false;

	TerrainCapture->CustomProjectionMatrix = OrthoMatrix;
	TerrainCapture->bUseCustomProjectionMatrix = true;
	TerrainCapture->SetWorldRotation(FRotator(90.f, 0.f, 0.f));
	TerrainCapture->SetRelativeLocation(CaptureDistanceFromOrigin);
	TerrainCapture->TextureTarget = TerrainRenderTarget;
	TerrainCapture->ShowFlags.Atmosphere = false;
	//TerrainCapture->ShowFlags.Cloud = false;
	TerrainCapture->ShowFlags.Fog = false;
	TerrainCapture->CompositeMode = ESceneCaptureCompositeMode::SCCM_Overwrite;
	TerrainCapture->PrimitiveRenderMode = ESceneCapturePrimitiveRenderMode::PRM_UseShowOnlyList;
	TerrainCapture->CaptureSource = ESceneCaptureSource::SCS_SceneDepth;
	TerrainCapture->bCaptureEveryFrame = false;
	TerrainCapture->bCaptureOnMovement = false;

	TerrainCapture->HiddenComponents.Empty();
	TerrainCapture->HideComponent(SkierMesh);

	TerrainCapture->ShowOnlyComponents.Empty();
	for (TObjectIterator<ALandscape> i; i; ++i) {
		if (i->IsA(ALandscape::StaticClass())) {
			TerrainCapture->ShowOnlyActorComponents(*i);
		}
	}

	CaptureWidth = 2048;
}

// Called every frame
void APathGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//if (TerrainActor) {
	MakeImprint();
	//}
}

FVector2D APathGenerator::GetUVFromWorld(FVector pos) {
	if (!TerrainActor) {
		return FVector2D(0.f, 0.f);
	}

	float u = (Origin.X - pos.X) / CaptureWidth;
	float v = (Origin.Y - pos.Y) / CaptureWidth;

	return FVector2D(u, v);
}

void APathGenerator::MakeImprint() {
	FVector SkierOrigin = SkierMesh->Bounds.Origin;
	FVector TerrainDimensions = TerrainExtent * 2;

	FHitResult HitResult;
	FCollisionQueryParams LineTraceCollisionParams;
	LineTraceCollisionParams.AddIgnoredActor(this);
	LineTraceCollisionParams.AddIgnoredComponent(Cast<UPrimitiveComponent>(SkierMesh));
	FVector LineTraceStart = SkierOrigin;
	FVector LineTraceEnd = LineTraceStart - 300.f * SkierMesh->GetUpVector();

	bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, LineTraceStart, LineTraceEnd, ECC_WorldStatic, LineTraceCollisionParams);

	if (!bHit || HitResult.Distance - SkierMesh->Bounds.BoxExtent.Z > SnowDepth) {
		return;
	}

	/*FVector PositionChange = SkierOrigin - PreviousPosition;
	float Distance = PositionChange.Length();

	float CapturePasses = Distance / 5.f;
	PositionChange = PositionChange / CapturePasses;*/

	FootprintCapture->SetWorldRotation(CaptureOrientation);
	FootprintCapture->SetWorldLocation(SkierOrigin + CaptureDistanceFromOrigin);

	TerrainCapture->SetWorldRotation(CaptureOrientation + FRotator(180.f, 0.f, 180.f));
	TerrainCapture->SetWorldLocation(SkierOrigin - CaptureDistanceFromOrigin);

	FootprintCapture->CaptureScene();
	TerrainCapture->CaptureScene();

	//FVector Pos = PreviousPosition;

	//for (int Inc = 0; Inc < FMath::CeilToInt(CapturePasses); Inc++) {
	//	Pos += PositionChange;

	//	UKismetMaterialLibrary::SetVectorParameterValue(GetWorld(), MPC, FName("Location"), FLinearColor(Pos.X, Pos.Y, 0.f, 0.f));
	//	UKismetMaterialLibrary::SetScalarParameterValue(GetWorld(), MPC, FName("Size"), 1000.f);
	//	TrailBlenderShaderInstance->SetTextureParameterValue(FName("SavedTrail"), TrailRenderTargetTemp);

	//	UKismetRenderingLibrary::DrawMaterialToRenderTarget(this, TrailRenderTarget, TrailBlenderShaderInstance);

	//	Swap(TrailRenderTargetTemp, TrailRenderTarget);
	//	//UKismetMaterialLibrary::SetVectorParameterValue(GetWorld(), MPC, FName("PreviousLocation"), FLinearColor(Pos.X - PositionChange.X, Pos.Y - PositionChange.Y, 0.f, 0.f));
	//}

	UKismetMaterialLibrary::SetVectorParameterValue(GetWorld(), MPC, FName("Location"), FLinearColor(SkierOrigin.X, SkierOrigin.Y, 0.f, 0.f));
	UKismetMaterialLibrary::SetScalarParameterValue(GetWorld(), MPC, FName("Size"), 1000.f);
	TrailBlenderShaderInstance->SetTextureParameterValue(FName("SavedTrail"), TrailRenderTargetTemp);

	UKismetRenderingLibrary::DrawMaterialToRenderTarget(this, TrailRenderTarget, TrailBlenderShaderInstance);

	Swap(TrailRenderTargetTemp, TrailRenderTarget);
	UKismetMaterialLibrary::SetVectorParameterValue(GetWorld(), MPC, FName("PreviousLocation"), FLinearColor(SkierOrigin.X, SkierOrigin.Y, 0.f, 0.f));
	PreviousPosition = SkierOrigin;
}

void APathGenerator::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) {

	if (!OtherActor->ActorHasTag("Snow") || OtherActor == TerrainActor) {
		return;
	}

	TerrainActor = OtherActor;
	TerrainActor->GetActorBounds(false, Origin, TerrainExtent);
}