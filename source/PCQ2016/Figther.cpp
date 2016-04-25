// Fill out your copyright notice in the Description page of Project Settings.

#include "PCQ2016.h"
#include "Figther.h"


// Sets default values
AFigther::AFigther()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AFigther::BeginPlay()
{
	Super::BeginPlay();
	
	m_gameMode = (APCQ2016GameMode*)GetWorld()->GetAuthGameMode();
	m_figtherID = m_gameMode->getNextFighterID();

	GetMesh()->SetMaterial(1, materials[m_figtherID]);

	initialPos = GetActorLocation();
	initialRot = GetActorRotation();
	 
	m_gameMode->getEventManager().subscribe("OnGameOver", this, &AFigther::onGameOver);

	m_gameOver = false;
}

void AFigther::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (m_gameMode)
	{
		m_gameMode->getEventManager().unsubscribe("OnGameOver", this, &AFigther::onGameOver);
	}
}

void AFigther::onGameOver(OnGameOverData *eventData)
{
	m_gameOver = true;

	UE_LOG(LogTemp, Warning, TEXT("GAME OVER FOR PLAYER %d"), (m_figtherID + 1));
}

// Called every frame
void AFigther::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	AFloatingPlatform *platform;

	switch (m_currentState)
	{
	case FigtherState::Idle:
		platform = checkPlatform();

		if (platform)
		{
			m_currentState = FigtherState::OnFloatingPlatform;

			platform->setOccupied(true);
			m_currentPlatform = platform;
		}

		break;

	case FigtherState::OnFloatingPlatform:
		platform = checkPlatform();

		if (!platform)
		{
			m_currentState = FigtherState::Idle;

			m_currentPlatform->setOccupied(false);
			m_currentPlatform = nullptr;
		}

		break;
	}

	if (m_totemCaptured && tickCooldown > 0)
	{
		tickCooldown -= DeltaTime;

		if (tickCooldown <= 0)
		{
			tickCooldown = 3;

			if (score < 200 && GetWorld()->GetLevelScriptActor() && GetWorld()->GetLevelScriptActor()->IsA(APCQ2016LevelScript::StaticClass()))
			{
				APCQ2016LevelScript *levelScript = Cast<APCQ2016LevelScript>(GetWorld()->GetLevelScriptActor());

				levelScript->onTotemTicked(new OnTotemTickedData(m_figtherID));

				score += 10;

				UE_LOG(LogTemp, Warning, TEXT("Incrementing score: %f"), score);

				if (score >= 200)
				{

					UE_LOG(LogTemp, Warning, TEXT("Score is 200!!!"));
					m_gameMode->getEventManager().raiseEvent("OnGameOver", new OnGameOverData(m_figtherID));
				}
			}
		}
	}
}

AFloatingPlatform *AFigther::checkPlatform()
{
	FCollisionQueryParams traceParams;
	traceParams.AddIgnoredActor(this);
	FVector startPos = GetCapsuleComponent()->GetComponentLocation() - FVector(0, 0, GetCapsuleComponent()->GetScaledCapsuleHalfHeight());
	FVector endPos = startPos - FVector(0, 0, 5);

	TArray<FHitResult> groundHits;
	GetWorld()->LineTraceMultiByChannel(groundHits, startPos, endPos, ECollisionChannel::ECC_WorldDynamic, traceParams);

	for (FHitResult hitResult : groundHits)
	{
		if (hitResult.Actor->IsA(AFloatingPlatform::StaticClass()))
		{
			AFloatingPlatform *platform = Cast<AFloatingPlatform>(hitResult.GetActor());

			//UE_LOG(LogTemp, Warning, TEXT("Found platform"));

			platform->BoxCollider->AddForceAtLocation(FVector(0, 0, platform->BoxCollider->GetMass() * -30), hitResult.ImpactPoint);

			return platform;
		}
	}
	
	return nullptr;
}

void AFigther::onTotemCaptured()
{
	m_totemCaptured = true;

	tickCooldown = 3;
}

void AFigther::onTotemLost()
{
	m_totemCaptured = false;
}

void AFigther::respawn()
{
	SetActorLocation(initialPos);
	SetActorRotation(initialRot);
}

/*
// Called to bind functionality to input
void AFigther::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

}*/

