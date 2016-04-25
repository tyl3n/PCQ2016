// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameMode.h"
#include "EventManager.h"
#include "PCQ2016GameMode.generated.h"

class FighterController;

UCLASS()
class PCQ2016_API APCQ2016GameMode : public AGameMode
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class AFighterController> FighterControllerClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class AFigther> FigtherClass;

	virtual void BeginPlay() override;
	virtual void Tick(float deltaTime) override;

	EventManager &getEventManager();

	int getNextFighterID();
	
private:
	EventManager m_eventManager;

	int32 m_currentFighterID = -1;
};
