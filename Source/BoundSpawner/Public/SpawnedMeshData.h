// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "SpawnedMeshData.generated.h"

USTRUCT(BlueprintType)
struct BOUNDSPAWNER_API FSpawnedMeshData : public FTableRowBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName meshName = FName("None");
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector position;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRotator rotation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float scale;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UStaticMesh> meshType;
};
