#pragma once

#include "CoreMinimal.h"
#include "Materials/MaterialExpressionCustomOutput.h"
#include "MaterialCompiler.h"

#include "ThermalMaterial.generated.h"

UCLASS(collapsecategories, hidecategories = Object)
class IRCAMERA_API UMaterialExpressionThermalMaterial : public UMaterialExpressionCustomOutput
{
	GENERATED_BODY()

public:
	// Inputs for thermal properties (e.g., emissivity, texture)
	UPROPERTY(EditAnywhere, Category = "Thermal Properties")
	FExpressionInput BaseThermalTexture;

	UPROPERTY(EditAnywhere, Category = "Thermal Properties")
	FExpressionInput ThermalEmissivity;

	UPROPERTY(EditAnywhere, Category = "Thermal Properties")
	FExpressionInput ThermalConductivity;

	UPROPERTY(EditAnywhere, Category = "Thermal Properties")
	FExpressionInput HeatCapacity;

	UPROPERTY(EditAnywhere, Category = "Thermal Properties")
	FExpressionInput SurfaceReflectivity;

	UPROPERTY(EditAnywhere, Category = "Thermal Properties")
	FExpressionInput ThermalAbsorptivity;

	UPROPERTY(EditAnywhere, Category = "Thermal Properties")
	FExpressionInput SurfaceTransmissivity;

	UPROPERTY(EditAnywhere, Category = "Thermal Properties")
	FExpressionInput Thickness;

	UPROPERTY(EditAnywhere, Category = "Thermal Properties")
	FExpressionInput SolarAbsorptivity;

	// Additional thermal-related properties can be added here
	UPROPERTY(EditAnywhere, Category = "Thermal Properties")
	float ThermalScale;

	// Constructor
	UMaterialExpressionThermalMaterial();

	// Overrides

#if WITH_EDITOR
	virtual int32 Compile(class FMaterialCompiler* Compiler, int32 OutputIndex) override;
	virtual void GetCaption(TArray<FString>& OutCaptions) const override;
	virtual FString GetFunctionName() const override;
#endif
};

