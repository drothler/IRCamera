// Copyright Epic Games, Inc. All Rights Reserved.

#include "IRCamera.h"
#include "ThermalShader.h"
#include "Interfaces/IPluginManager.h"
#include "Misc/Paths.h"

#define LOCTEXT_NAMESPACE "FIRCameraModule"



void FIRCameraModule::StartupModule()
{
	UE_LOG(LogTemp, Log, TEXT("Started IRCameraModule"));
	FString PluginShaderDir = FPaths::Combine(IPluginManager::Get().FindPlugin(TEXT("IRCamera"))->GetBaseDir(), TEXT("Shaders"));
	AddShaderSourceDirectoryMapping(TEXT("/Plugin/IRCamera"), PluginShaderDir);

}

void FIRCameraModule::ShutdownModule() 
{
}

void FIRCameraModule::TriggerRender(FRHICommandListImmediate& RHICmdList, FTexture2DRHIRef RenderTargetTexture, const FSceneView *InView) {
	ThermalShaderUtil::ForceRedScreen(RHICmdList, RenderTargetTexture, InView);
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FIRCameraModule, IRCamera)