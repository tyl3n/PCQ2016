/*
The buoyancy calculation technique was inspired from:
http://seawisphunter.com/blog/2015/11/24/simulating-buoyancy-part1/

Thanks to Shane Celis who assisted me generously by e-mail when I had questions

Twitter: @shanecelis
*/

#include "PCQ2016.h"
#include "FloatingPlatformSpawner.h"


AFloatingPlatformSpawner::AFloatingPlatformSpawner()
{
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;
}

void AFloatingPlatformSpawner::BeginPlay()
{
	Super::BeginPlay();

	// Caching
	m_world = GetWorld();

	FVector origin, boxExtent;
	GetActorBounds(false, origin, boxExtent);

	instantiateBoundingBoxCollider(boxExtent, origin);

	Destroy();
}

void AFloatingPlatformSpawner::instantiateBoundingBoxCollider(const FVector &boxExtent, const FVector &origin)
{
	FVector pos = GetActorLocation();
	FRotator rot = GetActorRotation();

	// We instantiate the bounding box

	if (!m_boundingBox)
	{
		m_boundingBox = NewObject<UBoxComponent>(this);

		m_boundingBox->SetBoxExtent(boxExtent);

		m_boundingBox->RegisterComponent();

		RootComponent = m_boundingBox;

		// We relocate the components to keep their relative positions and rotations
		FVector originOffset = origin - pos;
		SetActorLocation(pos + originOffset);
		SetActorRotation(rot);
		Mesh->SetRelativeLocationAndRotation(-originOffset, FRotator::ZeroRotator);

		Mesh->AttachTo(RootComponent);
	}

	AFloatingPlatform *FloatingPlatform = m_world->SpawnActor<AFloatingPlatform>(FloatingPlatformClass, GetActorLocation(), GetActorRotation());
	FloatingPlatform->initSpawnValues(Mesh->GetRelativeTransform().GetTranslation(), Mesh->GetComponentScale(), boxExtent, Mesh->GetStaticMesh(), WaterLevel);
}