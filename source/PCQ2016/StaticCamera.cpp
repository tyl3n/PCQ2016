// Fill out your copyright notice in the Description page of Project Settings.

#include "PCQ2016.h"
#include "StaticCamera.h"


// Sets default values
AStaticCamera::AStaticCamera()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	RootComponent = Camera;
}

// Called when the game starts or when spawned
void AStaticCamera::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AStaticCamera::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

