// Fill out your copyright notice in the Description page of Project Settings.


#include "BoundSpawnerWidget.h"

#include "SlateOptMacros.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SBoundSpawnerWidget::Construct(const FArguments& InArgs)
{
	//Custom settings object
	SettingsObject = GetMutableDefault<UBoundSpawnerSettings>();
	
	//Load property module
	FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");

	//Set Details view settings with FDetailsViewArgs
	FDetailsViewArgs Args;
	Args.bHideSelectionTip = false;
	Args.bAllowSearch = false;

	//Create property widget and set it's object to mutable version of spawner settings
	TSharedPtr<IDetailsView> SettingsWidget = PropertyModule.CreateDetailView(Args);
	SettingsWidget->SetObject(SettingsObject);

	SpawnButton = SNew(SButton)
		.HAlign(HAlign_Center)
		.VAlign(VAlign_Center)
		.Content()
		[
			SNew(STextBlock)
			.Text(FText::FromString(TEXT("Spawn")))
		];


	ChildSlot
	[
		SNew(SOverlay)
		+ SOverlay::Slot()
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Fill)
		[
			//Settings view
			SNew(SVerticalBox)
			+ SVerticalBox::Slot()
			.AutoHeight()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			[
				SettingsWidget.ToSharedRef()
			]

			+ SVerticalBox::Slot()
			.AutoHeight()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			[
				SNew(SSpacer)
				.Size(FVector2D(1.0f, 10.0f))
			]
			+ SVerticalBox::Slot()
			.AutoHeight()
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Top)
			[
				SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
				.HAlign(HAlign_Center)
				.VAlign(VAlign_Top)
				[
					//Spawn button
					SpawnButton.ToSharedRef()				
				]
			]
		]
	];
}

UBoundSpawnerSettings* SBoundSpawnerWidget::GetCustomSettings() const
{
	return SettingsObject;
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
