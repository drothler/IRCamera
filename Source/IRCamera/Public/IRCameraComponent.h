// Copyright (c) 2017 Computer Vision Center (CVC) at the Universitat Autonoma de Barcelona (UAB). This work is licensed under the terms of the MIT license. For a copy, see <https://opensource.org/licenses/MIT>.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneCaptureComponent2D.h"
#include "IRCameraComponent.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class IRCAMERA_API UIRCameraComponent : public USceneCaptureComponent2D 
{
	GENERATED_BODY()

public:
    // Thermal sensitivity (e.g., in Kelvin or a normalized value)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Thermal Camera")
    float ThermalSensitivity;

    // Temperature range (min and max values)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Thermal Camera")
    FVector2D TemperatureRange;

    // Spatial resolution (e.g., pixels per unit)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Thermal Camera")
    FVector2D SpatialResolution;

    // Emissivity (0.0 to 1.0)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Thermal Camera")
    float Emissivity;

    // Atmospheric attenuation factor
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Thermal Camera")
    float AtmosphericAttenuation;

    // Thermal response time (e.g., in milliseconds)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Thermal Camera")
    float ThermalResponseTime; 

    // Noise profile (e.g., variance or predefined options)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Thermal Camera")
    float NoiseProfile;

    // Calibration bias (offset to adjust readings)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Thermal Camera")
    float CalibrationBias;

    // Color palette (to map temperatures to colors)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Thermal Camera")
    UTexture* ColorPalette;

    void SetRenderTarget(UTextureRenderTarget2D* RenderTarget);
    UTextureRenderTarget2D* GetRenderTarget();
	
};
