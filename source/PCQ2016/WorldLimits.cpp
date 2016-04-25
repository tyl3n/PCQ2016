// Fill out your copyright notice in the Description page of Project Settings.

#include "PCQ2016.h"
#include "WorldLimits.h"


// Sets default values
AWorldLimits::AWorldLimits()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	worldLimits = 1000.0f;
	pullingForceStrength = 1000.0f;

}

// Called when the game starts or when spawned
void AWorldLimits::BeginPlay()
{
	Super::BeginPlay();
	world = GetWorld();
	
	
}

// Called every frame
void AWorldLimits::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
	for (TActorIterator<AFloatingPlatform> itr(world); itr; ++itr)
	{

		if(itr->IsA(AFloatingPlatform::StaticClass()))
		{
			if ((itr->GetActorLocation() - this->GetActorLocation()).Size2D()>= worldLimits) {
				itr->BoxCollider->AddImpulse(-(itr->GetActorLocation() - this->GetActorLocation()).GetSafeNormal() * pullingForceStrength*itr->BoxCollider->GetMass());
			}
		}
	}

}

