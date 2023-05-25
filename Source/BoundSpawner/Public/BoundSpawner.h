// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BoundSpawnerWidget.h"
#include "Modules/ModuleManager.h"

class FToolBarBuilder;
class FMenuBuilder;

class FBoundSpawnerModule : public IModuleInterface
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	
	void PluginButtonClicked();
	
private:
	TSharedPtr<class FUICommandList> PluginCommands;
	TSharedPtr<SBoundSpawnerWidget> BoundSpawnerWidget;
	TSharedPtr<UDataTable> SpawnDataTable;

	void RegisterMenus();
	TSharedRef<class SDockTab> OnSpawnPluginTab(const class FSpawnTabArgs& SpawnTabArgs);

	FReply OnSpawnButtonClicked();
	void SpawnInBounds(UStaticMesh* staticMesh, UBoundSpawnerSettings& settings);

private:
};
