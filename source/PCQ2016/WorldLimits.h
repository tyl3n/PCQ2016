// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "FloatingPlatform.h"
#include "GameFramework/Actor.h"
#include "WorldLimits.generated.h"


UCLASS()
class PCQ2016_API AWorldLimits : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWorldLimits();
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float worldLimits;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float pullingForceStrength;
	UWorld *world;
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	
	
};
