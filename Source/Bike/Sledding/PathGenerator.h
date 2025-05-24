// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Engine/Texture2D.h"
#include "PathGenerator.generated.h"

UCLASS()
class BIKE_API APathGenerator : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APathGenerator();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* SkierMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UTextureRenderTarget2D* ImprintRenderTarget;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UTextureRenderTarget2D* TerrainRenderTarget;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UTextureRenderTarget2D* TrailRenderTargetTemp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UTextureRenderTarget2D* TrailRenderTarget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UMaterialInterface* TrailBlenderShader;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USceneCaptureComponent2D* FootprintCapture;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USceneCaptureComponent2D* TerrainCapture;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UMaterialParameterCollection* MPC;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SnowDepth;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY()
	class UMaterialInstanceDynamic* SnowImprintShaderInstance;

	UPROPERTY()
	class UMaterialInstanceDynamic* TrailBlenderShaderInstance;

	UPROPERTY()
	FRotator CaptureOrientation;

	UPROPERTY()
	FVector CaptureDistanceFromOrigin;

	AActor* TerrainActor;
	FVector Origin;
	FVector TerrainExtent;
	float CaptureWidth;
	FVector PreviousPosition;

	FVector2D GetUVFromWorld(FVector pos);

	void MakeImprint();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "snow")
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
};
