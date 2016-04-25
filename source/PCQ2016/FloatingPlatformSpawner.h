// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "PCQ2016GameMode.h"
#include "Kismet/KismetMathLibrary.h"
#include "Matrix3x3.h"
#include "Matrix4x4.h"
#include "FloatingPlatform.h"
#include "FloatingPlatformSpawner.generated.h"

UCLASS()
class PCQ2016_API AFloatingPlatformSpawner : public AActor
{
	GENERATED_BODY()

public:
	enum class FloatingPlatformSpawnerState { Grounded, FreeFalling, Floating };

	UPROPERTY(VisibleAnywhere)
		class UStaticMeshComponent *Mesh;

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class AFloatingPlatform> FloatingPlatformClass;

	UPROPERTY(EditAnywhere)
		float WaterLevel;

	// Sets default values for this actor's properties
	AFloatingPlatformSpawner();

	virtual void BeginPlay() override;

private:
	TArray<FVector> m_unitVectors; // This is the i, j, k vectors
	UBoxComponent *m_boundingBox;
	UWorld *m_world;

	void instantiateBoundingBoxCollider(const FVector &boxExtent, const FVector &origin);
};
