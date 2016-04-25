// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "FloatingPlatform.h"
#include "Shockwave.generated.h"

UCLASS()
class PCQ2016_API AShockwave : public AActor
{
	GENERATED_BODY()
	
public:	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Physics)
		float radius;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Physics)
		float height;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Physics)
		float strength;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float maxRadius;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float radiusGrowRate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float heightDecreaseRate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float strengthDecreaseRate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float effectSize;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float lifeSpan;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float maxLifeSpan;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool isAttack;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		AActor* MyOwner;
	 UFUNCTION( BlueprintImplementableEvent, BlueprintCallable,Category = "Emmiter")
	void EmmiterReset();


	// Sets default values for this actor's properties
	AShockwave();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;
	void grow(float DeltaSeconds);
	UFUNCTION(BlueprintCallable, Category = AI)
	void detectCollisionForOneActor(AActor* testedActor);

	
	
};
