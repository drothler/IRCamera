#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraActor.h"
#include "IRCameraComponent.h" // Include your custom component header file
#include "ThermalCamera.generated.h"

UCLASS()
class IRCAMERA_API AThermalCamera : public ACameraActor
{
    GENERATED_BODY()

public:
    AThermalCamera();

    UIRCameraComponent* GetThermalCameraComponent() const {
        return ThermalCameraComponent;
    }


protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "IRCamera", meta = (AllowPrivateAccess = "true"))
    UIRCameraComponent* ThermalCameraComponent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IRCamera")
    UTextureRenderTarget2D* ThermalRenderTarget;

    virtual void BeginPlay() override;

private:
    void CreateAndAssignRenderTarget();
};