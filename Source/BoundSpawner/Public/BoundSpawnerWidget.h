// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BoundSpawnerSettings.h"
#include "Widgets/SCompoundWidget.h"

/**
 * 
 */
class BOUNDSPAWNER_API SBoundSpawnerWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SBoundSpawnerWidget)
		{
		}

	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

private:
	UBoundSpawnerSettings* SettingsObject = nullptr;

public:
	TSharedPtr<SButton> SpawnButton;
	
	UBoundSpawnerSettings* GetCustomSettings() const;
};
