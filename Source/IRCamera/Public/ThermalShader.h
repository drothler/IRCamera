#pragma once


class ThermalShaderUtil 
{
    public:
        static void ExecuteRenderPass(FRHICommandListImmediate& RHICmdList);

        static void ForceRedScreen(FRHICommandListImmediate& RHICmdList, FTexture2DRHIRef RenderTargetTexture, const FSceneView *InView);
};



