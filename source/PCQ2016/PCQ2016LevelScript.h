// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "PCQ2016GameMode.h"
#include "EventManager.h"
#include "OnTotemTickedData.h"
#include "OnGameOverData.h"
#include "PCQ2016LevelScript.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSkipMatineeDelegate, int32, figtherId);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGameOverDelegate, int32, winnerId);

UCLASS()
class PCQ2016_API APCQ2016LevelScript : public ALevelScriptActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APCQ2016LevelScript();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	// This event has to be listened in the blueprint
	UPROPERTY(BlueprintAssignable)
	FSkipMatineeDelegate OnPointTicked;

	UPROPERTY(BlueprintAssignable)
	FGameOverDelegate OnGameOver;


	void onTotemTicked(OnTotemTickedData *eventData);

private:
	APCQ2016GameMode *m_gameMode;

	void onGameOver(OnGameOverData *eventData);
	
};
