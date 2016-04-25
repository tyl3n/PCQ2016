// Fill out your copyright notice in the Description page of Project Settings.

#include "PCQ2016.h"
#include "FighterController.h"


void AFighterController::BeginPlay()
{
	Super::BeginPlay();

	AFigther *fighter = GetWorld()->SpawnActor<AFigther>(FighterClass);
	this->SetViewTarget(fighter, FViewTargetTransitionParams());
	this->Possess(fighter);
}

void AFighterController::setInitialSpawnValues(FVector loc, FRotator rot)
{
	return;
	if (!FighterClass) return;

	m_initialLoc = loc;
	m_initialRot = rot;
	UE_LOG(LogTemp, Warning, TEXT("Spawning figther"));
	AFigther *fighter = GetWorld()->SpawnActor<AFigther>(FighterClass, loc, rot);

	this->SetViewTarget(fighter, FViewTargetTransitionParams());
	this->Possess(fighter);
}