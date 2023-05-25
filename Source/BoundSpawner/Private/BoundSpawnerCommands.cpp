// Copyright Epic Games, Inc. All Rights Reserved.

#include "BoundSpawnerCommands.h"

#define LOCTEXT_NAMESPACE "FBoundSpawnerModule"

void FBoundSpawnerCommands::RegisterCommands()
{
	UI_COMMAND(OpenPluginWindow, "BoundSpawner", "Bring up BoundSpawner window", EUserInterfaceActionType::Button, FInputChord());
}

#undef LOCTEXT_NAMESPACE
