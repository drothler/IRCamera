// Copyright (c) 2017 Computer Vision Center (CVC) at the Universitat Autonoma de Barcelona (UAB). This work is licensed under the terms of the MIT license. For a copy, see <https://opensource.org/licenses/MIT>.


#include "ThermalCamera.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "IRCameraComponent.h"
#include "Camera/CameraAnim.h"

#define LOCTEXT_NAMESPACE "ThermalCameraActor"

// Sets default values
AThermalCamera::AThermalCamera()
{

	RootComponent = GetDefaultAttachComponent();

	ThermalCameraComponent = CreateDefaultSubobject<UIRCameraComponent>(TEXT("ThermalCameraComponent"));
	/* ThermalCameraComponent->FieldOfView = 90.0f;
	ThermalCameraComponent->bConstrainAspectRatio = true;
	ThermalCameraComponent->AspectRatio = 1.777778f; */
	ThermalCameraComponent->PostProcessBlendWeight = 1.0f;

	ThermalCameraComponent->SetupAttachment(RootComponent);

}

void AThermalCamera::BeginPlay()
{
    Super::BeginPlay();

    if (!ThermalRenderTarget)
    {
        CreateAndAssignRenderTarget();
    }
}

void AThermalCamera::CreateAndAssignRenderTarget()
{
    // Create the render target
    ThermalRenderTarget = NewObject<UTextureRenderTarget2D>(this, TEXT("ThermalRenderTarget"));
    if (ThermalRenderTarget)
    {
        // Set the render target properties
        ThermalRenderTarget->InitAutoFormat(1080, 1080); // Set resolution (adjust as needed)
        ThermalRenderTarget->ClearColor = FLinearColor::Black; // Set clear color (optional)
        ThermalRenderTarget->UpdateResourceImmediate(true); // Update the resource immediately

        // Assign the render target to the camera component
        if (ThermalCameraComponent)
        {
            ThermalCameraComponent->SetRenderTarget(ThermalRenderTarget);
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to create ThermalRenderTarget!"));
    }
}


#undef LOCTEXT_NAMESPACE
