// Fill out your copyright notice in the Description page of Project Settings.

#include "PCQ2016.h"
#include "Shockwave.h"


// Sets default values
AShockwave::AShockwave()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	radius = 0.0f;
	strength = 5000.0f;
	height = 1000.0f;
	effectSize = 40.0f;
	lifeSpan = 10.0f;
	maxLifeSpan = 10.0f;
	maxRadius = 700.0f;
	radiusGrowRate = 50.0f;
	heightDecreaseRate = 0.0f;
	strengthDecreaseRate = 0.0f;

}

// Called when the game starts or when spawned
void AShockwave::BeginPlay()
{
	Super::BeginPlay();
	radius = 0.0f;
}

// Called every frame
void AShockwave::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	grow(DeltaTime);

}
void AShockwave::grow(float DeltaTime)
{
	radius += radiusGrowRate*DeltaTime;
	if (strength > 0)
		strength -= strengthDecreaseRate*DeltaTime;
	if (strength > 0)
		height -= heightDecreaseRate*DeltaTime;
	lifeSpan -= DeltaTime;
	//UE_LOG(LogTemp, Warning, TEXT("LifeSpan : %f"), lifeSpan);
	if (isAttack && radius>maxRadius)
	{
		this->Destroy();
	}
	if (lifeSpan <= 0)
	{
		this->Destroy();
	}

}
void AShockwave::detectCollisionForOneActor(AActor* testedActor)
{
		if (testedActor != MyOwner)
		{
			FVector ShockwaveUp = this->GetActorUpVector();
			float waveZ = this->GetActorLocation().Z;
			float waveHeight = this->GetActorLocation().Z + height;
			FVector origin, extent;
			testedActor->GetActorBounds(true, origin,extent);

			float topCapsule = (testedActor->GetActorLocation().Z + extent.Z);
			float botomCapuslue = (testedActor->GetActorLocation().Z - extent.Z);

			
			if (testedActor->IsA(AFloatingPlatform::StaticClass()))
			{
				/*UE_LOG(LogTemp, Warning, TEXT("waveZ: %f"), waveZ);
				UE_LOG(LogTemp, Warning, TEXT("waveHeight: %f"), waveHeight);
				UE_LOG(LogTemp, Warning, TEXT("topCapsule: %f"), topCapsule);
				UE_LOG(LogTemp, Warning, TEXT("botomCapuslue: %f"), botomCapuslue);*/
			}
			

			if ((testedActor->GetActorLocation() - this->GetActorLocation()).Size2D() <= radius + effectSize &&
				(testedActor->GetActorLocation() - this->GetActorLocation()).Size2D() >= radius - effectSize &&
				(waveZ >= (botomCapuslue) && waveHeight <= topCapsule
				|| waveHeight > topCapsule && waveZ < topCapsule || waveHeight > botomCapuslue && waveZ< botomCapuslue))
			{

				
				FVector lauchVector = -(this->GetActorLocation() - testedActor->GetActorLocation()).GetSafeNormal()*strength;

				if (testedActor->IsA(ACharacter::StaticClass()))
				{
					//UE_LOG(LogTemp, Warning, TEXT("Character Detected"));
					//UE_LOG(LogTemp, Warning, TEXT("Collision avec un character"));
					ACharacter* testedCharacter = Cast<ACharacter>(testedActor);
					lauchVector.Z = 50;
					testedCharacter->LaunchCharacter(lauchVector * 0.0005,true, true);
					//testedCharacter->GetCharacterMovement->AddImpulse(lauchVector);
				}
				/*if (testedActor->IsA(AFloatingPlatform::StaticClass()))
				{
					//UE_LOG(LogTemp, Warning, TEXT("AAAAAAAAAAAAAAAAAAAAAA"));
					//UE_LOG(LogTemp, Warning, TEXT("Character Detected"));
					//UE_LOG(LogTemp, Warning, TEXT("Collision avec un plateforme"));
					AFloatingPlatform* testedPlatform = Cast<AFloatingPlatform>(testedActor);
					testedPlatform->BoxCollider->AddImpulse(lauchVector);

				}*/


			
	    }
	}

}


