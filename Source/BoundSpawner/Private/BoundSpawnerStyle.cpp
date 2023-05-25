// Copyright Epic Games, Inc. All Rights Reserved.

#include "BoundSpawnerStyle.h"
#include "Styling/SlateStyleRegistry.h"
#include "Framework/Application/SlateApplication.h"
#include "Slate/SlateGameResources.h"
#include "Interfaces/IPluginManager.h"
#include "Styling/SlateStyleMacros.h"

#define RootToContentDir Style->RootToContentDir

TSharedPtr<FSlateStyleSet> FBoundSpawnerStyle::StyleInstance = nullptr;

void FBoundSpawnerStyle::Initialize()
{
	if (!StyleInstance.IsValid())
	{
		StyleInstance = Create();
		FSlateStyleRegistry::RegisterSlateStyle(*StyleInstance);
	}
}

void FBoundSpawnerStyle::Shutdown()
{
	FSlateStyleRegistry::UnRegisterSlateStyle(*StyleInstance);
	ensure(StyleInstance.IsUnique());
	StyleInstance.Reset();
}

FName FBoundSpawnerStyle::GetStyleSetName()
{
	static FName StyleSetName(TEXT("BoundSpawnerStyle"));
	return StyleSetName;
}

const FVector2D Icon16x16(16.0f, 16.0f);
const FVector2D Icon32x32(32.0f, 32.0f);

TSharedRef< FSlateStyleSet > FBoundSpawnerStyle::Create()
{
	TSharedRef< FSlateStyleSet > Style = MakeShareable(new FSlateStyleSet("BoundSpawnerStyle"));
	Style->SetContentRoot(IPluginManager::Get().FindPlugin("BoundSpawner")->GetBaseDir() / TEXT("Resources"));

	Style->Set("BoundSpawner.OpenPluginWindow", new IMAGE_BRUSH_SVG(TEXT("ToolButton"), Icon32x32));

	return Style;
}

void FBoundSpawnerStyle::ReloadTextures()
{
	if (FSlateApplication::IsInitialized())
	{
		FSlateApplication::Get().GetRenderer()->ReloadTextureResources();
	}
}

const ISlateStyle& FBoundSpawnerStyle::Get()
{
	return *StyleInstance;
}
