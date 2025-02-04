// Copyright Epic Games, Inc. All Rights Reserved.

#include "ThermalRendererModule.h"
#include "ThermalShader.h"
#include "Interfaces/IPluginManager.h"
#include "Misc/Paths.h"
#include "RHI.h"

#define LOCTEXT_NAMESPACE "FThermalRendererModule"



void FThermalRendererModule::StartupModule()
{
	IRendererModule& SceneRendererModule = FModuleManager::LoadModuleChecked<IRendererModule>("Renderer");

	UE_LOG(LogTemp, Log, TEXT("Started up ThermalRendererModule"));

    ThermalViewExtension = FSceneViewExtensions::NewExtension<FThermalViewExtension>();
}

void FThermalRendererModule::ShutdownModule() 
{
	ThermalViewExtension->ReleaseRenderTarget();
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FThermalRendererModule, ThermalRenderer)