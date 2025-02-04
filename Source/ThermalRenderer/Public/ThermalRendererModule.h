// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "RenderGraphResources.h"
#include "Runtime/Engine/Classes/Engine/TextureRenderTarget2D.h"
#include "ThermalViewExtension.h"



class FThermalRendererModule : public IModuleInterface
{
public:

	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	static inline FThermalRendererModule& Get()
	{
		return FModuleManager::LoadModuleChecked<FThermalRendererModule>("ThermalRenderer");
	}

	static inline bool IsAvailable()
	{
		return FModuleManager::Get().IsModuleLoaded("ThermalRenderer");
	}

	TSharedPtr<FThermalViewExtension, ESPMode::ThreadSafe> ThermalViewExtension;
};
