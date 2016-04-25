// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerController.h"
#include "Figther.h"
#include "FighterController.generated.h"

/**
 * 
 */
UCLASS()
class PCQ2016_API AFighterController : public APlayerController
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class AFigther> FighterClass;

	virtual void BeginPlay() override;
	
	void setInitialSpawnValues(FVector loc, FRotator rot);

private:
	FVector m_initialLoc;
	FRotator m_initialRot;
};
