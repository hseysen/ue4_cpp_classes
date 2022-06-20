#include "ObjectSpawner.h"
#define DEBUG true

AObjectSpawner::AObjectSpawner()
{
	PrimaryActorTick.bCanEverTick = false;

	SplineComponent = CreateDefaultSubobject<USplineComponent>("Spline");
	if (SplineComponent) SetRootComponent(SplineComponent);

}

// Called when the game starts or when spawned
void AObjectSpawner::BeginPlay()
{
	Super::BeginPlay();
	spline_length = SplineComponent->GetSplineLength();
	if(spline_length < MaxOffset) MaxOffset = spline_length;
	SpawnRandomInstances();
}

// Called every frame
void AObjectSpawner::Tick(float DeltaTime) {}

void AObjectSpawner::SpawnRandomInstances() {
	if (MeshesToSpawn.Num() == 0) return;

	float TotalDist = 0.0f;
	if (KeepLooping)
	{
		for (int i = 0; i < InstancedMeshes.Num(); i++) InstancedMeshes[i]->Destroy();
		InstancedMeshes.Empty();
	}

	int instances = FMath::RandRange(MinInstances, MaxInstances);

	if (DEBUG)
	{
		FString temp0 = FString::FromInt(FMath::FloorToInt(spline_length));
		FString temp1 = FString::FromInt(instances);
		UE_LOG(LogTemp, Warning, TEXT("Spline Length: %s"), *temp0);
		UE_LOG(LogTemp, Warning, TEXT("Instances to spawn: %s"), *temp1)
	}

	for (int i = 0; i < instances; i++)
	{
		float random_distance = FMath::RandRange(MinOffset, MaxOffset);
		TotalDist += random_distance;
		int mesh_index = FMath::RandRange(0, MeshesToSpawn.Num() - 1);
		UStaticMesh* MeshToSpawn = MeshesToSpawn[mesh_index];
		FBox mesh_bbox = MeshToSpawn->GetBoundingBox();
		FVector mesh_sizes = mesh_bbox.Max - mesh_bbox.Min;
		FVector Location = SplineComponent->GetLocationAtDistanceAlongSpline(TotalDist, ESplineCoordinateSpace::World);
		FRotator Rotation = SplineComponent->GetRotationAtDistanceAlongSpline(TotalDist, ESplineCoordinateSpace::World);
		
		if (DEBUG)
		{
			FString temp2 = FString::FromInt(FMath::FloorToInt(random_distance));
			UE_LOG(LogTemp, Warning, TEXT("Selected Offset for instance #%d, which is %s (index %d): %s"), i+1, *MeshToSpawn->GetName(), mesh_index, *temp2);
		}

		if (TotalDist >= spline_length) break;

		AStaticMeshActor* MyNewActor = GetWorld()->SpawnActor<AStaticMeshActor>(AStaticMeshActor::StaticClass());
		InstancedMeshes.Insert(MyNewActor, 0);
		MyNewActor->SetMobility(EComponentMobility::Movable);
		MyNewActor->SetActorLocation(Location);
		MyNewActor->SetActorRotation(Rotation);
		UStaticMeshComponent* MeshComponent = MyNewActor->GetStaticMeshComponent();
		if (MeshComponent) MeshComponent->SetStaticMesh(MeshToSpawn);
		MyNewActor->SetRootComponent(SplineComponent);

		if (ConsiderObjectDimensions)
		{
			float max_size_to_consider = mesh_sizes.Size();
			TotalDist += max_size_to_consider;
		}
	}

	if (KeepLooping)
	{
		FTimerHandle TimeHandler;
		GetWorldTimerManager().SetTimer(TimeHandler, this, &AObjectSpawner::SpawnRandomInstances, TimeInterval, false);
	}
}

