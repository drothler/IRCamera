#pragma once

#include "CoreMinimal.h"
#include "ThermalMaterial.h"

class FThermalMaterialRenderProxy : public FMaterialRenderProxy
{
public:

	const FMaterialRenderProxy* const Parent;
	
	FThermalMaterialRenderProxy(const UMaterialExpressionThermalMaterial* InThermalMaterial, const FMaterialRenderProxy* InParentProxy)
		: Parent(InParentProxy),
		ThermalMaterial(InThermalMaterial)
	{
		
	}

private:
	// Pointer to the thermal material expression
	const UMaterialExpressionThermalMaterial* ThermalMaterial;

	
};