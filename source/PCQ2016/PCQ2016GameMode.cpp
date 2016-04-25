// Fill out your copyright notice in the Description page of Project Settings.

#include "PCQ2016.h"
#include "PCQ2016GameMode.h"
#include "FighterController.h"

void APCQ2016GameMode::BeginPlay()
{
	Super::BeginPlay();

	//TSubclassOf<APlayerStart> playerStartClass;
	TArray<APlayerStart*> playerStartActors;
	//UGameplayStatics::GetAllActorsOfClass(GetWorld(), playerStartClass, playerStartActors);

	for (TActorIterator<APlayerStart> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		playerStartActors.Add(Cast<APlayerStart>(*ActorItr));
	}

	if (!FighterControllerClass) return;

	for (AActor *playerStartActor : playerStartActors)
	{
		APlayerStart *playerStart = Cast<APlayerStart>(playerStartActor);

		FVector spawnPos = playerStart->GetActorLocation();
		FRotator spawnRot = playerStart->GetActorRotation();

		/*
		AFighterController *controller = GetWorld()->SpawnActor<AFighterController>(FighterControllerClass, spawnPos, spawnRot);
		AFigther *figther = GetWorld()->SpawnActor<AFigther>(FigtherClass, spawnPos, spawnRot);
		*/
	}

	//FSystemResolution::RequestResolutionChange(5000, 1080, EWindowMode::WindowedFullscreen);
}

void APCQ2016GameMode::Tick(float deltaTime)
{
	Super::Tick(deltaTime);
}

int32 APCQ2016GameMode::getNextFighterID()
{
	m_currentFighterID += 1;

	UE_LOG(LogTemp, Warning, TEXT("%d"), m_currentFighterID);

	return m_currentFighterID;
}

EventManager& APCQ2016GameMode::getEventManager()
{
	return m_eventManager;
}