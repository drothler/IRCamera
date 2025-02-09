#pragma once

#include "CoreMinimal.h"
#include "SceneViewExtension.h"
#include "IRCamera.h"
#include "CanvasTypes.h"
#include "ThermalCamera.h"
#include "IRCameraComponent.h"
#include "ThermalRendering.h"

class FThermalViewExtension : public FSceneViewExtensionBase
{
public:
    FThermalViewExtension(const FAutoRegister& AutoRegister);
    virtual ~FThermalViewExtension() override
    {
        UE_LOG(LogTemp, Log, TEXT("Thermal View Extension Destroyed"));
    }

    virtual void SetupViewFamily(FSceneViewFamily& InViewFamily) override {
    }
    virtual void SetupView(FSceneViewFamily& InViewFamily, FSceneView& InView) override {
    }
    virtual void BeginRenderViewFamily(FSceneViewFamily& InViewFamily) override {
    }

    // Override Empty
    virtual void PostRenderViewFamily_RenderThread(FRHICommandListImmediate& RHICmdList, FSceneViewFamily& InViewFamily) override {};
    virtual void PostRenderBasePass_RenderThread(FRHICommandListImmediate& RHICmdList, FSceneView& InView) override;
    virtual void PreRenderViewFamily_RenderThread(FRHICommandListImmediate& RHICmdList, FSceneViewFamily& InViewFamily) override {};
    virtual void PreRenderView_RenderThread(FRHICommandListImmediate& RHICmdList, FSceneView& InView) override {};

    // Entry Point for Thermal Render Process
    virtual void PrePostProcessPass_RenderThread(FRDGBuilder& GraphBuilder, const FSceneView& View, const FPostProcessingInputs& Inputs) override {};
    
    virtual bool IsActiveThisFrame(FViewport* InViewport) const override;

    void ReleaseRenderTarget();

    virtual int32 GetPriority() const override
    {
        return 0; // Default priority
    }

    void InitializeRenderTarget(FRHICommandListImmediate& RHICmdList);

    bool SetupCameraRenderTexture(FSceneView& InView);

    bool UpdateCameraRenderTarget(FSceneView& InView);

    void TestRenderTarget(FRHICommandListImmediate& RHICmdList, FTexture2DRHIRef RenderTargetTexture, const FSceneView *InView);

    //TRefCountPtr<IPooledRenderTarget> ThermalRenderTarget;

    bool bIsRenderTargetInitialized = false;

    FTexture2DRHIRef ThermalTexture;

    UTextureRenderTarget2D* RenderTarget;

    FTextureRenderTarget2DResource* RenderTargetResource;
};