// Fill out your copyright notice in the Description page of Project Settings.

#include "PCQ2016.h"
#include "PCQ2016LevelScript.h"


// Sets default values
APCQ2016LevelScript::APCQ2016LevelScript()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APCQ2016LevelScript::BeginPlay()
{
	Super::BeginPlay();

	m_gameMode = (APCQ2016GameMode*)GetWorld()->GetAuthGameMode();
	m_gameMode->getEventManager().subscribe("OnGameOver", this, &APCQ2016LevelScript::onGameOver);
}

void APCQ2016LevelScript::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (m_gameMode)
	{
		m_gameMode->getEventManager().unsubscribe("OnGameOver", this, &APCQ2016LevelScript::onGameOver);
	}
}

void APCQ2016LevelScript::onTotemTicked(OnTotemTickedData *eventData)
{
	UE_LOG(LogTemp, Warning, TEXT("ON TOTEM TICKED LEVEL SCRIPT"));

	if (OnPointTicked.IsBound())
	{
		UE_LOG(LogTemp, Warning, TEXT("IS BOUND!!!"));
		// This is to alert the blueprint listeners
		OnPointTicked.Broadcast(eventData->getFigtherId());
	}
}

void APCQ2016LevelScript::onGameOver(OnGameOverData *eventData)
{
	UE_LOG(LogTemp, Warning, TEXT("THE WINNER IS PLAYER %d !!!"), (eventData->getWinnerId() + 1));

	if (OnGameOver.IsBound())
	{
		OnGameOver.Broadcast(eventData->getWinnerId());

		UE_LOG(LogTemp, Warning, TEXT("BOUND!!! Winner ID: %d"), eventData->getWinnerId());
	}
}