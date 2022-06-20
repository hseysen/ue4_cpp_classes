// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SplineComponent.h"
#include "Engine/StaticMeshActor.h"
#include "ObjectSpawner.generated.h"

UCLASS()
class SIMULATIONS_API AObjectSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AObjectSpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Minimum instances to be spawned
	UPROPERTY(EditAnywhere, Category = "Spawning") int MinInstances = 1;
	// Maximum instances to be spawned
	UPROPERTY(EditAnywhere, Category = "Spawning") int MaxInstances = 5;
	// Minimum distance offset between two consecutive spawned instances
	UPROPERTY(EditAnywhere, Category = "Spawning") float MinOffset = 10.0f;
	// Maximum distance offset between two consecutive spawned instances
	UPROPERTY(EditAnywhere, Category = "Spawning") float MaxOffset = 1000.0f;
	// Whether or not to consider the object dimensions. Adds extra distance offset if true,
	// in order to make sure that two consecutive instances do not collide
	UPROPERTY(EditAnywhere, Category = "Spawning") bool ConsiderObjectDimensions = true;
	// Whether or not to keep looping after spawning instances.
	UPROPERTY(EditAnywhere, Category = "Spawning") bool KeepLooping = true;
	// The time period between spawn loops
	UPROPERTY(EditAnywhere, Category = "Spawning") float TimeInterval = 2.0f;
	// Meshes to randomly choose from when spawning
	UPROPERTY(EditAnywhere, Category = "Spawning") TArray<UStaticMesh*> MeshesToSpawn;

	float spline_length;
	USplineComponent* SplineComponent;
	TArray<AStaticMeshActor*> InstancedMeshes;
	void SpawnRandomInstances();
};
