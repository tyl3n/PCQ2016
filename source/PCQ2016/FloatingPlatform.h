// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "PCQ2016GameMode.h"
#include "Kismet/KismetMathLibrary.h"
#include "Matrix3x3.h"
#include "Matrix4x4.h"
#include "Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h"
#include "FloatingPlatform.generated.h"

UCLASS()
class PCQ2016_API AFloatingPlatform : public AActor
{
	GENERATED_BODY()

public:
	enum class FloatingPlatformState { Grounded, FreeFalling, Floating };

	UPROPERTY(VisibleAnywhere)
	class UBoxComponent *BoxCollider;

	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent *Mesh;

	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent *MeshTest;

	// Sets default values for this actor's properties
	AFloatingPlatform();

	virtual void BeginPlay() override;
	virtual void BeginDestroy() override;
	virtual void Tick(float deltaSeconds) override;

	void initSpawnValues(FVector meshLocation, FVector meshScale, FVector boxExtent, UStaticMesh *staticMesh, float waterLevel);

	void setOccupied(bool occupied);
	bool getOccupied();

private:
	// Buoyancy force = Volume submerged * density of liquid * gravity
	// Viscosity force = 6 * Pi * dynamic viscosity * radius * velocity
	// Drag force = 0.5 * drag coefficient * density of liquid * cross sectional area (i.e. area of bottom face) * velocity^2

	// 0.001027 is the real density of cold water, but since the cube is submerged too much for the current solution,
	// we cheat a little so that the block doesn't get completely submerged when quickly changing the water level
	const float M_SEA_WATER_DENSITY = 0.00130;// 0.001027; // kg/cm^3
	const float M_ICE_BLOCK_DENSITY = 0.000917; // kg/cm^3
	const float M_COLD_WATER_VISCOSITY = 0.00179; // Pa*s (Viscosity of the water just before its freezing point)
	const float M_CUBE_DRAG_COEFFICIENT = 1.05;// 1.05;

	float m_waterLevel;

	bool m_isFirstTick;
	bool m_wasPushed;

	bool m_hasAlertedObjects;

	TArray<FVector> m_unitVectors; // This is the i, j, k vectors

	UWorld *m_world;
	FVector m_blockSize; // cm

	float m_blockVolume; // cm^3

	FVector m_lastCheckpointPosition;
	FRotator m_lastCheckpointRotation;
	FVector m_lastCheckpointVelocity;
	FVector m_lastCheckpointAngularVelocity;
	bool m_lastCheckpointIsInWater;

	float getBuoyancyZ(float submergedVolume);
	float getViscosityZ();
	float getDragZ();

	void computeBlockVolume(const FVector &boxExtent);

	float getSubmergedUnitBlockVolume(FVector planeNormal, FVector p0); // cm^3
	float getTetrahedronVolume(FVector planeNormal, FVector planePoint); // cm^3
	float getTetrahedronCenterOfMassElement(float element, FVector planeNormal, FVector planePoint);
	FVector getCenterOfBuoyancy(FVector unitPlaneNormal, FVector unitPlanePoint, Matrix3x3 swapTransform, Matrix3x3 firstQuadrantTransform);
	void applyBuoyancyForces();
	void floatOnWater();

	Matrix3x3 getSwapTransform(FVector normal);

	bool m_isOccupied;
};
