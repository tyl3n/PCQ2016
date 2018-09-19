// Fill out your copyright notice in the Description page of Project Settings.

#include "PCQ2016.h"
#include "KillZone.h"


// Sets default values
AKillZone::AKillZone()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Collider = CreateDefaultSubobject<UBoxComponent>(TEXT("Collider"));
	RootComponent = Collider;
}

// Called when the game starts or when spawned
void AKillZone::BeginPlay()
{
	Super::BeginPlay();
	Collider->OnComponentBeginOverlap.AddDynamic(this, &AKillZone::OnBeginOverlap);
}

// Called every frame
void AKillZone::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

void AKillZone::OnBeginOverlap(class UPrimitiveComponent* overlappedComponent ,class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor->IsA(AFigther::StaticClass()))
	{
		UE_LOG(LogTemp, Warning, TEXT("A figther has fallen"));

		TArray<AFloatingPlatform*> platforms;

		for (TActorIterator<AFloatingPlatform> ActorItr(GetWorld()); ActorItr; ++ActorItr)
		{
			AFloatingPlatform *platform = Cast<AFloatingPlatform>(*ActorItr);

			if (!platform->getOccupied())
			{
				platforms.Add(platform);
			}
		}

		int platformIndex = FMath::RandRange(0, platforms.Num() - 1);

		FVector respawnPos = platforms[platformIndex]->GetActorLocation() + FVector(0, 0, 150);


		Cast<AFigther>(OtherActor)->respawn();
	}
}