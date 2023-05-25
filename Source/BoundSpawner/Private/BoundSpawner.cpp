// Copyright Epic Games, Inc. All Rights Reserved.

#include "BoundSpawner.h"
#include "BoundSpawnerStyle.h"
#include "BoundSpawnerCommands.h"
#include "BoundSpawnerWidget.h"
#include "EditorAssetLibrary.h"
#include "ShaderCompiler.h"
#include "SpawnedMeshData.h"
#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"
#include "ToolMenus.h"
#include "Engine/AssetManager.h"
#include "Engine/StaticMeshActor.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Subsystems/UnrealEditorSubsystem.h"

static const FName BoundSpawnerTabName("BoundSpawner");

#define LOCTEXT_NAMESPACE "FBoundSpawnerModule"

void FBoundSpawnerModule::StartupModule()
{
	FBoundSpawnerStyle::Initialize();
	FBoundSpawnerStyle::ReloadTextures();

	FBoundSpawnerCommands::Register();
	
	PluginCommands = MakeShareable(new FUICommandList);
	PluginCommands->MapAction(
		FBoundSpawnerCommands::Get().OpenPluginWindow,
		FExecuteAction::CreateRaw(this, &FBoundSpawnerModule::PluginButtonClicked),
		FCanExecuteAction());

	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FBoundSpawnerModule::RegisterMenus));
	
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(BoundSpawnerTabName, FOnSpawnTab::CreateRaw(this, &FBoundSpawnerModule::OnSpawnPluginTab))
		.SetDisplayName(LOCTEXT("FBoundSpawnerTabTitle", "Bound Spawner"))
		.SetMenuType(ETabSpawnerMenuType::Hidden);
}


void FBoundSpawnerModule::ShutdownModule()
{
	UToolMenus::UnRegisterStartupCallback(this);
	UToolMenus::UnregisterOwner(this);
	FBoundSpawnerStyle::Shutdown();
	FBoundSpawnerCommands::Unregister();
	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(BoundSpawnerTabName);
}

void FBoundSpawnerModule::PluginButtonClicked()
{
	FGlobalTabmanager::Get()->TryInvokeTab(BoundSpawnerTabName);
}

void FBoundSpawnerModule::RegisterMenus()
{
	// Owner will be used for cleanup in call to UToolMenus::UnregisterOwner
	FToolMenuOwnerScoped OwnerScoped(this);

	{
		UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Tools");
		{
			FToolMenuSection& Section = Menu->FindOrAddSection("Level");
			Section.AddMenuEntryWithCommandList(FBoundSpawnerCommands::Get().OpenPluginWindow, PluginCommands);
		}
	}

	{
		UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar.PlayToolBar");
		{
			FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("PluginTools");
			{
				FToolMenuEntry& Entry = Section.AddEntry(FToolMenuEntry::InitToolBarButton(FBoundSpawnerCommands::Get().OpenPluginWindow));
				Entry.SetCommandList(PluginCommands);
			}
		}
	}
}

TSharedRef<SDockTab> FBoundSpawnerModule::OnSpawnPluginTab(const FSpawnTabArgs& SpawnTabArgs)
{
	//Make Bound Spawner widget
	BoundSpawnerWidget = SNew(SBoundSpawnerWidget);
	
	BoundSpawnerWidget->SpawnButton->SetOnClicked(FOnClicked::CreateRaw(this, &FBoundSpawnerModule::OnSpawnButtonClicked));

	return SNew(SDockTab)
		.TabRole(ETabRole::NomadTab)
		[ BoundSpawnerWidget.ToSharedRef() ];
}





FReply FBoundSpawnerModule::OnSpawnButtonClicked()
{
	//Get tool settings from widget
	UBoundSpawnerSettings* settings = BoundSpawnerWidget->GetCustomSettings();
	const TArray<FSoftObjectPath> assetsToLoad = {
		settings->mesh.ToSoftObjectPath(),
		settings->dataTable.ToSoftObjectPath(),
		settings->material.ToSoftObjectPath()
	};

	//Load needed assets async
	UAssetManager::Get().GetStreamableManager().RequestAsyncLoad(assetsToLoad,
		[this, settings]()
		{
			UStaticMesh* mesh = settings->mesh.Get();
			if(IsValid(mesh))
				SpawnInBounds(mesh, *settings);
		});
	
	return FReply::Handled();
}

void FBoundSpawnerModule::SpawnInBounds(UStaticMesh* staticMesh, UBoundSpawnerSettings& settings)
{
	if(settings.spawnAmount == 0)
	{
		UE_LOG(LogTemp, Display, TEXT("No meshes were spawned. Spawn Amount was set to zero."));
		return;
	}

	//Get World
	UUnrealEditorSubsystem* EditorSubsystem = GEditor->GetEditorSubsystem<UUnrealEditorSubsystem>();
	const UObject* worldContext = EditorSubsystem->GetEditorWorld();
	UWorld* World = GEngine->GetWorldFromContextObject(worldContext, EGetWorldErrorMode::LogAndReturnNull);
	if(!World)
	{
		UE_LOG(LogTemp, Error, TEXT("Bound Spawner can't find world to spawn mesh into."));
		return;
	}

	if(GEditor)
	{
		//Start transaction for undo/redo
		GEditor->BeginTransaction(FText::FromString("Spawn meshes in bounds"));
		UDataTable* dataTable = settings.dataTable.LoadSynchronous(); //Load if it wasn't loaded until now
		dataTable->Modify();

		UE_LOG(LogTemp, Display, TEXT("Spawning %i meshes."), settings.spawnAmount);
	
		for (int i = 0; i < settings.spawnAmount; ++i)
		{
			//Generate random position, rotation and scale based on tool settings
			FSpawnedMeshData spawnedMeshData;
			spawnedMeshData.position = FVector(FMath::RandRange(settings.positionRange.Min.X, settings.positionRange.Max.X),
											   FMath::RandRange(settings.positionRange.Min.Y, settings.positionRange.Max.Y),
											   FMath::RandRange(settings.positionRange.Min.Z, settings.positionRange.Max.Z));
			spawnedMeshData.rotation = FRotator(FMath::RandRange(settings.rotationRange.Min.X, settings.rotationRange.Max.X),
												 FMath::RandRange(settings.rotationRange.Min.Y, settings.rotationRange.Max.Y),
												 FMath::RandRange(settings.rotationRange.Min.Z, settings.rotationRange.Max.Z));
			spawnedMeshData.scale = FMath::RandRange(settings.scaleRange.GetLowerBoundValue(), settings.scaleRange.GetUpperBoundValue());
			spawnedMeshData.meshType = settings.mesh;

			//Generate a unique name to not overlap other entry names in data table 
			const FString actorName = FString::Printf(TEXT("%s_%s"), *staticMesh->GetName(), *FGuid::NewGuid().ToString());

			//Spawn actor and set position, rotation and scale
			AStaticMeshActor* meshActor = World->SpawnActor<AStaticMeshActor>(AStaticMeshActor::StaticClass(), spawnedMeshData.position, spawnedMeshData.rotation);
			meshActor->SetActorRelativeScale3D(FVector(spawnedMeshData.scale));
			meshActor->SetActorLabel(actorName, true);
			meshActor->SetMobility(EComponentMobility::Stationary);

			//Set static mesh component
			UStaticMeshComponent* meshComponent = meshActor->GetStaticMeshComponent();
			if(meshComponent)
			{
				meshComponent->SetStaticMesh(staticMesh);

				//Create dynamic material and set a random color
				UMaterialInstanceDynamic* MaterialInstance = UMaterialInstanceDynamic::Create(settings.material.Get(), NULL);
				meshComponent->SetMaterial(0, MaterialInstance);
				if(MaterialInstance)
				{
					FRandomStream RandomStream(FMath::Rand());
					FVector RandomBaseColor = FVector(RandomStream.FRandRange(0.0f, 1.0f),
													  RandomStream.FRandRange(0.0f, 1.0f),
													  RandomStream.FRandRange(0.0f, 1.0f));
					MaterialInstance->SetVectorParameterValue(TEXT("Color"), FLinearColor(RandomBaseColor));
					meshComponent->MarkRenderInstancesDirty();
				}
			}
			settings.dataTable->AddRow(*actorName, spawnedMeshData);
			meshActor->Modify();
		}
		UEditorAssetLibrary::SaveLoadedAsset(dataTable, false);

		//End transaction for undo/redo
		GEditor->EndTransaction();
	}
	
}



#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FBoundSpawnerModule, BoundSpawner)