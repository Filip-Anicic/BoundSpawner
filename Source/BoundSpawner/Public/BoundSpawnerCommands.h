// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "BoundSpawnerStyle.h"

class FBoundSpawnerCommands : public TCommands<FBoundSpawnerCommands>
{
public:

	FBoundSpawnerCommands()
		: TCommands<FBoundSpawnerCommands>(TEXT("BoundSpawner"), NSLOCTEXT("Contexts", "BoundSpawner", "BoundSpawner Plugin"), NAME_None, FBoundSpawnerStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > OpenPluginWindow;
};