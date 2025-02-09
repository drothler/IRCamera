#include "ThermalMaterial.h"

UMaterialExpressionThermalMaterial::UMaterialExpressionThermalMaterial()
{
    // Default value for the thermal scale
    ThermalScale = 1.0f;
}

#if WITH_EDITOR
int32 UMaterialExpressionThermalMaterial::Compile(class FMaterialCompiler* Compiler, int32 OutputIndex)
{    if (!Compiler)
    {
        return INDEX_NONE; // Return if the compiler is null
    }

    // Compile each input property or use a default value if not connected.
    const int32 BaseThermalTextureCode = BaseThermalTexture.Expression ? BaseThermalTexture.Compile(Compiler) : Compiler->Constant(0.0f);

    
    const int32 ThermalEmissivityCode = ThermalEmissivity.Expression ? ThermalEmissivity.Compile(Compiler) : Compiler->Constant(0.8f); // Default emissivity
    /* const int32 ThermalConductivityCode = ThermalConductivity.Expression ? ThermalConductivity.Compile(Compiler) : Compiler->Constant(0.0f);
    const int32 HeatCapacityCode = HeatCapacity.Expression ? HeatCapacity.Compile(Compiler) : Compiler->Constant(1.0f);
    const int32 SurfaceReflectivityCode = SurfaceReflectivity.Expression ? SurfaceReflectivity.Compile(Compiler) : Compiler->Constant(0.0f);
    const int32 ThermalAbsorptivityCode = ThermalAbsorptivity.Expression ? ThermalAbsorptivity.Compile(Compiler) : Compiler->Constant(1.0f);
    const int32 SurfaceTransmissivityCode = SurfaceTransmissivity.Expression ? SurfaceTransmissivity.Compile(Compiler) : Compiler->Constant(0.0f);
    const int32 ThicknessCode = Thickness.Expression ? Thickness.Compile(Compiler) : Compiler->Constant(1.0f);
    const int32 SolarAbsorptivityCode = SolarAbsorptivity.Expression ? SolarAbsorptivity.Compile(Compiler) : Compiler->Constant(1.0f);

    // Apply ThermalScale as a multiplier to the final result.
    const int32 ThermalScaleCode = Compiler->Constant(ThermalScale);

    // Combine all compiled inputs into a single value, for example using an additive model.
    int32 CombinedThermalProperties = Compiler->Add(
    Compiler->Mul(BaseThermalTextureCode, ThermalEmissivityCode),  // Base texture weighted by emissivity
    Compiler->Mul(SurfaceReflectivityCode, Compiler->Add(ThermalConductivityCode, HeatCapacityCode)) // Reflectivity interacts with conductivity and capacity
    );

    // Further combine with absorptivity, transmissivity, and other thermal effects.
    CombinedThermalProperties = Compiler->Add(
    CombinedThermalProperties,
    Compiler->Mul(SolarAbsorptivityCode, Compiler->Add(ThermalAbsorptivityCode, SurfaceTransmissivityCode))
    );

    // Factor in thickness and thermal scale.
    CombinedThermalProperties = Compiler->Mul(
    CombinedThermalProperties,
    Compiler->Mul(ThicknessCode, ThermalScaleCode)
    );

    // Return the combined result.
    return CombinedThermalProperties; */

    Compiler->CustomOutput(this, 0, BaseThermalTextureCode);
    Compiler->CustomOutput(this, 1, ThermalEmissivityCode);

    return Compiler->Constant(0);
}


void UMaterialExpressionThermalMaterial::GetCaption(TArray<FString>& OutCaptions) const
{
    OutCaptions.Add(TEXT("Thermal Material"));
}

FString UMaterialExpressionThermalMaterial::GetFunctionName() const
{
    return TEXT("ThermalOutput");
}

#endif

