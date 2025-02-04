// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "RenderGraphResources.h"



class IRCAMERA_API FIRCameraModule : public IModuleInterface
{
public:
	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	static inline FIRCameraModule& Get()
	{
		return FModuleManager::LoadModuleChecked<FIRCameraModule>("IRCamera");
	}

	static inline bool IsAvailable()
	{
		return FModuleManager::Get().IsModuleLoaded("IRCamera");
	}

	void TriggerRender(FRHICommandListImmediate& RHICmdList, FTexture2DRHIRef RenderTargetTexture, const FSceneView *InView);
};
