/*

The buoyancy calculation technique was inspired from:
http://seawisphunter.com/blog/2015/11/24/simulating-buoyancy-part1/

Thanks to Shane Celis who assisted me generously by e-mail when I had questions

Twitter: @shanecelis

*/

#include "PCQ2016.h"
#include "FloatingPlatform.h"


// Gravity force = Cube volume * density of cube * gravity
// Buoyancy force = Volume submerged * density of water * gravity
// Gravity: 980cm/s^2
// Ice cube density: 0.000917kg/cm^3

AFloatingPlatform::AFloatingPlatform()
{
	PrimaryActorTick.bCanEverTick = true;

	BoxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollider"));
	RootComponent = BoxCollider;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->AttachTo(RootComponent);

	MeshTest = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshTest"));
	MeshTest->AttachTo(RootComponent);
}

void AFloatingPlatform::BeginPlay()
{
	Super::BeginPlay();

	m_world = GetWorld();
	m_wasPushed = false;

	/*
	BoxCollider->SetSimulatePhysics(true);
	BoxCollider->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	
	BoxCollider->SetAngularDamping(1);*/
}

void AFloatingPlatform::initSpawnValues(FVector meshLocation, FVector meshScale, FVector boxExtent, UStaticMesh *staticMesh, float waterLevel)
{
	BoxCollider->SetBoxExtent(boxExtent);

	

	// Caching
	m_world = GetWorld();

	Mesh->SetStaticMesh(staticMesh);
	Mesh->SetRelativeLocation(meshLocation);

	MeshTest->SetStaticMesh(staticMesh);
	MeshTest->SetRelativeLocation(meshLocation);

	FVector newCOM = BoxCollider->GetComponentLocation() - BoxCollider->GetCenterOfMass();

	BoxCollider->SetCenterOfMass(newCOM);

	Mesh->SetRelativeScale3D(meshScale);
	MeshTest->SetRelativeScale3D(meshScale);

	FVector meshOrigin, meshBoxExtent;
	float sphereRadius;

	UKismetSystemLibrary::GetComponentBounds(Mesh, meshOrigin, meshBoxExtent, sphereRadius);

	computeBlockVolume(meshBoxExtent);

	FVector origin2, boxExtent2;
	float radius2;

	UKismetSystemLibrary::GetComponentBounds(Mesh, origin2, boxExtent2, radius2);

	FVector centerOfMassOffset = origin2 - Mesh->GetCenterOfMass();

	m_waterLevel = waterLevel;
}

void AFloatingPlatform::BeginDestroy()
{
	Super::BeginDestroy();
}

void AFloatingPlatform::computeBlockVolume(const FVector &boxExtent)
{
	m_blockSize = boxExtent * 2;
	m_blockVolume = m_blockSize.X * m_blockSize.Y * m_blockSize.Z;

	BoxCollider->SetMassOverrideInKg(NAME_None, m_blockVolume * M_ICE_BLOCK_DENSITY);
	Mesh->SetMassOverrideInKg(NAME_None, m_blockVolume * M_ICE_BLOCK_DENSITY);

	FVector tableInit[] = { FVector(1, 0, 0), FVector(0, 1, 0), FVector(0, 0, 1) };
	m_unitVectors.Append(tableInit, 3);
}

void AFloatingPlatform::Tick(float deltaSeconds)
{
	// TODO: Improve code after prototype has been tested, all temporary for now

	Super::Tick(deltaSeconds);

	floatOnWater();

	FVector dir = -GetActorLocation();
	dir.Z = 0;
	dir = dir.GetSafeNormal();

	if (GetActorLocation().Size2D() > 1500)
	{
		BoxCollider->AddImpulse(dir * 0.005 * GetActorLocation().Size2D() * BoxCollider->GetMass() + dir * 10000);
	}
	else
	{
		BoxCollider->AddImpulse(-dir * 1000 * (1 / GetActorLocation().Size2D()) * BoxCollider->GetMass() - dir * 2000);
	}
}

// When the block is entering the water, we simulate a force to push it back afloat
void AFloatingPlatform::floatOnWater()
{
	applyBuoyancyForces();
}

void AFloatingPlatform::applyBuoyancyForces()
{
	// For our calculations, we rotate the system so that the block in lying flat and the water plane is skewed
	FRotator cuboidRot = BoxCollider->GetComponentRotation();
	FVector cuboidPlaneNormal = FVector::UpVector;
	FVector cuboidLocation = GetActorLocation();

	FVector cuboidPlanePoint = FVector(cuboidLocation.X, cuboidLocation.Y, m_waterLevel);

	Matrix4x4 cuboidToUnitTrans = Matrix4x4::translationMatrix(-cuboidLocation);
	Matrix4x4 cuboidToUnitRot = Matrix4x4::rotationMatrix(cuboidRot.GetInverse());
	Matrix4x4 cuboidToUnitScale = Matrix4x4::scaleMatrix(FVector(1 / m_blockSize.X, 1 / m_blockSize.Y, 1 / m_blockSize.Z));

	/*
	A normal vector needs a different scale transform than a point since it's only a direction without a location,
	and it actually scales inversely proportional to the point. So, we needs to get the transpose of the inverse
	for non-uniform scaling. This is not a problem for rotation or translation transforms since there is not "stretching",
	i.e. proportions are met.
	Reference: http://oldwww.acm.org/tog/resources/RTNews/html/rtnews1a.html#art4
	*/
	Matrix4x4 cuboidToUnitNormalScale = Matrix4x4::scaleMatrix(m_blockSize).transpose();

	Matrix4x4 cuboidToUnitTransform = cuboidToUnitScale * cuboidToUnitRot * cuboidToUnitTrans;
	Matrix4x4 cuboidToUnitNormalTransform = cuboidToUnitNormalScale * cuboidToUnitRot;

	FVector unitPlaneNormal = FVector(cuboidToUnitNormalTransform * FVector4(cuboidPlaneNormal, 0));
	unitPlaneNormal.Normalize(); // The scale may un-normalize it

	FVector unitPlanePoint = FVector(cuboidToUnitTransform * FVector4(cuboidPlanePoint, 1));

	FVector prevUnitPlanePoint = unitPlanePoint;
	FVector prevUnitPlaneNormal = unitPlaneNormal;

	Matrix3x3 swapTransform = getSwapTransform(unitPlaneNormal);
	Matrix3x3 firstQuadrantTransform = Matrix3x3::getFirstQuadrantTransform(unitPlaneNormal);
	Matrix3x3 reorderTransform = swapTransform * firstQuadrantTransform;
	unitPlaneNormal = reorderTransform * unitPlaneNormal;
	unitPlanePoint = reorderTransform * unitPlanePoint;

	unitPlanePoint += FVector(0.5, 0.5, 0.5);

	// Ugly patch because, apparently, the buoyancy method doesn't work when one element of the normal is zero
	if (FMath::IsNearlyZero(unitPlaneNormal.X, 0.001f))
	{
		unitPlaneNormal.X = 0.001;
	}

	if (FMath::IsNearlyZero(unitPlaneNormal.Y, 0.001f))
	{
		unitPlaneNormal.Y = 0.001;
	}

	if (FMath::IsNearlyZero(unitPlaneNormal.Z, 0.001f))
	{
		unitPlaneNormal.Z = 0.001;
	}

	float submergedVolume = getSubmergedUnitBlockVolume(unitPlaneNormal, unitPlanePoint) * m_blockVolume;

	// This guard is necessary since the overlap event is triggered the frame before the actual overlap
	if (submergedVolume > 0)
	{
		FVector totalForce = FVector(0, 0, getBuoyancyZ(submergedVolume) + getViscosityZ() + getDragZ());

		FVector centerOfBuoyancy = getCenterOfBuoyancy(unitPlaneNormal, unitPlanePoint, swapTransform, firstQuadrantTransform);

		BoxCollider->AddForceAtLocation(totalForce, centerOfBuoyancy);
	}
}

FVector AFloatingPlatform::getCenterOfBuoyancy(FVector unitPlaneNormal, FVector unitPlanePoint, Matrix3x3 swapTransform, Matrix3x3 firstQuadrantTransform)
{
	float d = FMath::Abs(FVector::DotProduct(unitPlaneNormal, unitPlanePoint));

	if (d < 0 || d > FVector::DotProduct(unitPlaneNormal, FVector(1, 1, 1))) return GetActorLocation();

	FVector baseCenter = d / 4 * unitPlaneNormal; // The center of the big tetrahedron without corrections

	FVector otherCenters = FVector::ZeroVector;

	for (int i = 0; i < 3; i++)
	{
		
		// We check if there are any overcounted tetrahedra
		if (unitPlaneNormal[i] != 0 && d / unitPlaneNormal[i] - 1 >= 0)
		{
			otherCenters[i] -= getTetrahedronCenterOfMassElement(unitPlaneNormal[i], unitPlaneNormal, unitPlanePoint - m_unitVectors[i]);
		}
		
		// We check if there are any undercounted tetrahedra
		if (unitPlaneNormal[(i + 1) % 3] != 0 && (d - unitPlaneNormal[i]) / unitPlaneNormal[(i + 1) % 3] - 1 >= 0)
		{
			otherCenters[i] += getTetrahedronCenterOfMassElement(unitPlaneNormal[i], unitPlaneNormal, unitPlanePoint - (m_unitVectors[i] + m_unitVectors[(i + 1) % 3]));
		}
	}

	FVector unitCenterOfBuoyancy = baseCenter + otherCenters;

	FVector centerOfBuoyancy = unitCenterOfBuoyancy;// -FVector(0.5, 0.5, 0.5);

	
	centerOfBuoyancy.X *= -1;
	centerOfBuoyancy.Y *= -1;
	centerOfBuoyancy.Z *= -1;

	/*
	Matrix4x4 adjustTrans = Matrix4x4::translationMatrix(FVector(-0.5, -0.5, -0.5));
	centerOfBuoyancy = FVector(adjustTrans * FVector4(centerOfBuoyancy, 1));*/

	Matrix3x3 reorderTransform = firstQuadrantTransform * swapTransform;
	centerOfBuoyancy = reorderTransform * centerOfBuoyancy;
	
	Matrix4x4 unitToCuboidTrans = Matrix4x4::translationMatrix(GetActorLocation());
	Matrix4x4 unitToCuboidRot = Matrix4x4::rotationMatrix(BoxCollider->GetComponentRotation());
	Matrix4x4 unitToCuboidScale = Matrix4x4::scaleMatrix(m_blockSize);

	Matrix4x4 unitToCuboidTransform = unitToCuboidTrans * unitToCuboidScale;

	centerOfBuoyancy = FVector(unitToCuboidTransform * FVector4(centerOfBuoyancy, 1));

	//DrawDebugLine(GetWorld(), centerOfBuoyancy, centerOfBuoyancy + FVector(0, 0, 600), FColor::Red);

	/*
	UE_LOG(LogTemp, Warning, TEXT("%s"), *firstQuadrantTransform[0].ToString());
	UE_LOG(LogTemp, Warning, TEXT("%s"), *firstQuadrantTransform[1].ToString());
	UE_LOG(LogTemp, Warning, TEXT("%s"), *firstQuadrantTransform[2].ToString());


	UE_LOG(LogTemp, Warning, TEXT("%s"), *unitCenterOfBuoyancy.ToString());
	UE_LOG(LogTemp, Warning, TEXT("%s"), *centerOfBuoyancy.ToString());
	UE_LOG(LogTemp, Warning, TEXT("****************"));*/

	//return GetActorLocation();

	return centerOfBuoyancy;
}

// If there are 0 elements in the vector (except for Y), we have to swap them since the integration order
// for the volume is "dz dx dy". So we order the zeros at the start of the vector when possible
Matrix3x3 AFloatingPlatform::getSwapTransform(FVector normal)
{
	Matrix3x3 swapTransform = Matrix3x3::identity;

	if (normal.X == 0 && normal.Y != 0 && normal.Z != 0)
	{
		// We want to have the Y as 0

		swapTransform = Matrix3x3::swapXYTransform;
	}
	else if (normal.X != 0 && normal.Y != 0 && normal.Z == 0)
	{
		// We want to have the Y as 0

		swapTransform = Matrix3x3::swapYZTransform;
	}
	else if (normal.Z == 0 && normal.X != 0 && normal.Y == 0)
	{
		// We want to have the X, Y as 0

		swapTransform = Matrix3x3::swapXZTransform;
	}
	else if (normal.Z == 0 && normal.X == 0 && normal.Y != 0)
	{
		// We want to have the X, Y as 0

		swapTransform = Matrix3x3::swapYZTransform;
	}

	return swapTransform;
}

float AFloatingPlatform::getSubmergedUnitBlockVolume(FVector planeNormal, FVector p0)
{
	Matrix3x3 swapTransform = getSwapTransform(planeNormal);
	Matrix3x3 firstQuadrantTransform = Matrix3x3::getFirstQuadrantTransform(planeNormal);
	Matrix3x3 reorderTransform = swapTransform * firstQuadrantTransform;
	planeNormal = reorderTransform * planeNormal;
	p0 = reorderTransform * p0;

	float d = FMath::Abs(FVector::DotProduct(planeNormal, p0));

	// If the block is not in the water yet
	if (FVector::DotProduct(planeNormal, p0) < 0) return 0;

	// If the block is completely under water
	if (d > FVector::DotProduct(planeNormal, FVector(1, 1, 1))) return 1;

	float baseVol = getTetrahedronVolume(planeNormal, p0); // The volume without corrections
	float volCorrection = 0; // The overcounted or undercounted volumes of tetrahedra

	for (int i = 0; i < 3; i++)
	{
		// We check if there are any overcounted tetrahedra
		if (planeNormal[i] != 0 && d / planeNormal[i] - 1 >= 0)
		{
			volCorrection -= getTetrahedronVolume(planeNormal, p0 - m_unitVectors[i]);
		}

		// We check if there are any undercounted tetrahedra
		if (planeNormal[(i + 1) % 3] != 0 && (d - planeNormal[i]) / planeNormal[(i + 1) % 3] - 1 >= 0)
		{
			volCorrection += getTetrahedronVolume(planeNormal, p0 - (m_unitVectors[i] + m_unitVectors[(i + 1) % 3]));
		}
	}

	float unitCubeSubmergedVolume = baseVol + volCorrection;

	return unitCubeSubmergedVolume;
}

float AFloatingPlatform::getTetrahedronVolume(FVector planeNormal, FVector planePoint)
{
	Matrix3x3 swapTransform = getSwapTransform(planeNormal);
	Matrix3x3 firstQuadrantTransform = Matrix3x3::getFirstQuadrantTransform(planeNormal);
	Matrix3x3 reorderTransform = swapTransform * firstQuadrantTransform;
	planeNormal = reorderTransform * planeNormal;
	planePoint = reorderTransform * planePoint;

	float d = FVector::DotProduct(planeNormal, planePoint);
	float a = planeNormal.X;
	float b = planeNormal.Y;
	float c = planeNormal.Z;

	// Since we reordered the normal elements earlier, we only have to do 3 cases instead of 7

	float vol = 0;

	if (a != 0 && b != 0 && c != 0)
	{
		vol = (d*d*d) / (6 * a*b*c);
	}
	else if (a == 0 && b == 0)
	{
		vol = d / c;
	}
	else if (b == 0)
	{
		vol = (d*d) / (2 * a*c);
	}

	return vol;
}

float AFloatingPlatform::getTetrahedronCenterOfMassElement(float element, FVector planeNormal, FVector planePoint)
{
	float d = FVector::DotProduct(planeNormal, planePoint);

	return d / 4 * element;
}

float AFloatingPlatform::getBuoyancyZ(float submergedVolume)
{
	// Buoyancy force = Volume submerged * density of water * gravity

	return submergedVolume * M_SEA_WATER_DENSITY * -m_world->GetGravityZ();
}


float AFloatingPlatform::getViscosityZ()
{
	// Since this force is negligible for large objects, we can remove it if there are performance issues or too many blocks in the level

	// Viscosity force = 6 * Pi * dynamic viscosity * radius * velocity
	float viscosityZ = 6 * PI * M_COLD_WATER_VISCOSITY * m_blockSize.Z / 2 * BoxCollider->GetPhysicsLinearVelocity().Z;

	// The formula gives us Pa*cm^2 (since Unreal units are centimeters), so we need to convert it
	// to Pa*m^2 (which are equal to newton or kg*m*s^-2), and then to kg*cm*s^-2
	viscosityZ /= m_blockSize.Z;

	return viscosityZ;
}

float AFloatingPlatform::getDragZ()
{
	// Drag force = 0.5 * drag coefficient * density of liquid * cross sectional area (i.e. area of bottom face) * velocity^2

	float velocityZ = BoxCollider->GetPhysicsLinearVelocity().Z;

	float dragZ = 0.5 * M_CUBE_DRAG_COEFFICIENT * M_SEA_WATER_DENSITY * m_blockSize.X * m_blockSize.Y * -FMath::Sign(velocityZ) * velocityZ * velocityZ;

	return dragZ;
}

void AFloatingPlatform::setOccupied(bool occupied)
{
	m_isOccupied = occupied;
}

bool AFloatingPlatform::getOccupied()
{
	return m_isOccupied;
}