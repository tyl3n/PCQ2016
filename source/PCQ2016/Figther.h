// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "FloatingPlatform.h"
#include "EventManager.h"
#include "OnTotemTickedData.h"
#include "PCQ2016LevelScript.h"
#include "OnGameOverData.h"
#include "Figther.generated.h"

UCLASS()
class PCQ2016_API AFigther : public ACharacter
{
	GENERATED_BODY()

public:
	enum class FigtherState { Idle, OnFloatingPlatform};

	// Sets default values for this character's properties
	AFigther();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Called to bind functionality to input
	//virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	UFUNCTION(BlueprintCallable, Category = "Totem")
	void respawn();

	UFUNCTION(BlueprintCallable, Category = "Totem")
	void onTotemCaptured();

	UFUNCTION(BlueprintCallable, Category = "Totem")
	void onTotemLost();

	UPROPERTY(EditDefaultsOnly)
	TArray<UMaterial *> materials;

	UPROPERTY(EditDefaultsOnly)
	TArray<UMaterial *> godModeMaterials;

	UPROPERTY(BlueprintReadOnly)
		bool m_gameOver;

private:
	FigtherState m_currentState;

	AFloatingPlatform *m_currentPlatform;

	APCQ2016GameMode *m_gameMode;

	AFloatingPlatform *checkPlatform();

	int m_figtherID;

	bool m_totemCaptured;

	float tickCooldown = 0;

	float score;

	FVector initialPos;
	FRotator initialRot;

	

	void onGameOver(OnGameOverData *eventData);
};
