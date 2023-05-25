// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "BoundSpawnerSettings.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, config = EditorPerProjectUserSettings)
class BOUNDSPAWNER_API UBoundSpawnerSettings : public UObject
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, Category="Settings", Config, meta=(RequiredAssetDataTags="RowStructure=SpawnedMeshData"))
	TSoftObjectPtr<UDataTable> dataTable = TSoftObjectPtr<UDataTable>(FSoftObjectPath("/BoundSpawner/DT_SpawnedMeshes.DT_SpawnedMeshes"));
	
	UPROPERTY(EditAnywhere, Category="Settings", Config, meta=(RowType=SpawnedMeshData))
	uint8 spawnAmount = 5;

	UPROPERTY(EditAnywhere, Category="Settings", Config, meta=(RowType=SpawnedMeshData))
	TSoftObjectPtr<UStaticMesh> mesh = TSoftObjectPtr<UStaticMesh>(FSoftObjectPath("/Engine/VREditor/BasicMeshes/SM_Cube_01.SM_Cube_01"));

	UPROPERTY(EditAnywhere, Category="Settings", Config)
	FBox positionRange = FBox(FVector(-100.0f, -100.0f, 0.0f), FVector(100.0f));

	UPROPERTY(EditAnywhere, Category="Settings", Config)
	FBox rotationRange = FBox(FVector(0.0f), FVector(360.0f));

	UPROPERTY(EditAnywhere, Category="Settings", Config)
	FFloatRange scaleRange = FFloatRange(0.25f, 1.0f);
	
};
