// Copyright (c) 2017 Computer Vision Center (CVC) at the Universitat Autonoma de Barcelona (UAB). This work is licensed under the terms of the MIT license. For a copy, see <https://opensource.org/licenses/MIT>.


#include "IRCameraComponent.h"
#include "Engine/TextureRenderTarget2D.h"


void UIRCameraComponent::SetRenderTarget(UTextureRenderTarget2D* RenderTarget)
{
    if (RenderTarget)
    {
        // Assign the render target to the scene capture component
        TextureTarget = RenderTarget;
        //UpdateContent(); // Force the capture component to update
    }
}

UTextureRenderTarget2D* UIRCameraComponent::GetRenderTarget()
{
    // Assign the render target to the scene capture component
    return TextureTarget;
    
}

