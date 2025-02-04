#include "ThermalViewExtension.h"
#include "RenderGraphUtils.h"
#include "ClearQuad.h"
#include "RenderTargetPool.h"


TRefCountPtr<IPooledRenderTarget> ThermalRenderTarget;
FPooledRenderTargetDesc RenderTargetDesc;

FThermalViewExtension::FThermalViewExtension(const FAutoRegister& AutoRegister)
    : FSceneViewExtensionBase(AutoRegister)
{
    UE_LOG(LogTemp, Log, TEXT("Thermal View Extension Constructed"));
    
    ENQUEUE_RENDER_COMMAND(InitializeRenderTargetCommand)(
        [this](FRHICommandListImmediate& RHICmdList)
        {
            InitializeRenderTarget(RHICmdList);
        });
}

void FThermalViewExtension::InitializeRenderTarget(FRHICommandListImmediate& RHICmdList)
{
    // Define the texture creation parameters
    FRHIResourceCreateInfo CreateInfo(TEXT("ThermalRenderTexture"));
    CreateInfo.ClearValueBinding = FClearValueBinding(FLinearColor::Red);
    
    // Create a 2D texture
    ThermalTexture = RHICreateTexture2D(
        1080,                                 // Texture width
        1080,                                // Texture height
        PF_B8G8R8A8,                           // Pixel format (adjust as needed)
        1,                                     // Number of mip levels
        1,                                     // Number of samples (multisampling, use 1 if not needed)
        TexCreate_RenderTargetable | TexCreate_ShaderResource,             
        CreateInfo                             // Additional resource creation info
    );

    if (ThermalTexture.IsValid())
    {
        UE_LOG(LogTemp, Log, TEXT("ThermalTexture successfully created"));
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Failed to create ThermalTexture"));
    }
}

void FThermalViewExtension::ReleaseRenderTarget()
{
    if (ThermalRenderTarget.IsValid())
    {
        ThermalRenderTarget.SafeRelease();
        UE_LOG(LogTemp, Log, TEXT("ThermalRenderTarget released"));
    }
}

bool FThermalViewExtension::SetupCameraRenderTexture(FSceneView& InView)
{
    RenderTarget = NewObject<UTextureRenderTarget2D>();

    if (RenderTarget)
    {
        // Initialize the render target with the same properties as the RHI texture
        RenderTarget->InitAutoFormat(1080, 1080); // Set resolution (adjust as needed)
        RenderTarget->ClearColor = FLinearColor::Black; // Set clear color (optional)
        RenderTarget->UpdateResourceImmediate(true); // Update the resource immediately

        // Link the RHI texture to the UTextureRenderTarget2D
        RenderTargetResource = (FTextureRenderTarget2DResource*)RenderTarget->GetRenderTargetResource();
        if (RenderTargetResource)
        {
            RenderTargetResource->TextureRHI = ThermalTexture;

            UE_LOG(LogTemp, Log, TEXT("Linked RHI texture to UTextureRenderTarget2D"));
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Failed to get RenderTargetResource"));
            return false;
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Failed to create UTextureRenderTarget2D"));
        return false;
    }

    return true;
}

bool FThermalViewExtension::UpdateCameraRenderTarget(FSceneView& InView) {
    const AActor* ViewActor = InView.ViewActor;
    if (!ViewActor || !ViewActor->IsA<AThermalCamera>()){
        return false;
    } 
    const AThermalCamera* ThermalCamera = Cast<AThermalCamera>(ViewActor);

    UIRCameraComponent* ThermalCameraComponent = ThermalCamera->GetThermalCameraComponent();

    if (ThermalCameraComponent->GetRenderTarget() == RenderTarget) {
        UE_LOG(LogTemp, Log, TEXT("Render target is already assigned to ThermalCameraComponent"));
        return true;
    }
    check(RenderTarget);
    ThermalCameraComponent->SetRenderTarget(RenderTarget);
    UE_LOG(LogTemp, Log, TEXT("Assigned render target to ThermalCameraComponent"));

    return true;
}

void FThermalViewExtension::PreRenderView_RenderThread(FRHICommandListImmediate& RHICmdList, FSceneView& InView)
{/* 
    // Ensure we're on the render thread
    check(IsInRenderingThread());
    
    if(!bIsRenderTargetInitialized){
        bIsRenderTargetInitialized = SetupCameraRenderTexture(InView);
    } 

    if(!bIsRenderTargetInitialized) {
        //UE_LOG(LogTemp, Error, TEXT("Could not initialized Render Target, aborting render."));
        return;
    }

     if(!UpdateCameraRenderTarget(InView))
    {
        UE_LOG(LogTemp, Log, TEXT("No Thermal Camera active."));
        return;
    } 

     UE_LOG(LogTemp, Log, TEXT("Updated Camera Render Target."));

    FRHITexture* Texture = ThermalTexture.GetReference();
    if (Texture)
    {
        FRHITransitionInfo TransitionInfo(Texture, ERHIAccess::Unknown, ERHIAccess::RTV);
        RHICmdList.Transition(TransitionInfo);
    } 
 
    FRHIRenderPassInfo RenderPassInfo(ThermalTexture, ERenderTargetActions::Load_Store);

    //UE_LOG(LogTemp, Log, TEXT("Created RenderPassInfo"));

    RHICmdList.BeginRenderPass(RenderPassInfo, TEXT("ThermalViewExtension"));
    {
        TestRenderTarget(RHICmdList);
        //UE_LOG(LogTemp, Log, TEXT("Performed Frame Render."));
    }
    RHICmdList.EndRenderPass();  */ 
}

void FThermalViewExtension::PreRenderViewFamily_RenderThread(FRHICommandListImmediate& RHICmdList, FSceneViewFamily& InViewFamily) 
{


    UE_LOG(LogTemp, Log, TEXT("Post Render"));
    
        UE_LOG(LogTemp, Log, TEXT("Trying to Render Red"));
        // Get the render target from the view family
        FTexture2DRHIRef RenderTargetTexture = InViewFamily.RenderTarget->GetRenderTargetTexture();

        

        for (int32 ViewIndex = 0; ViewIndex < InViewFamily.Views.Num(); ++ViewIndex)
        {
            TestRenderTarget(RHICmdList, RenderTargetTexture, InViewFamily.Views[ViewIndex]);
        }
        
    
}


void FThermalViewExtension::TestRenderTarget(FRHICommandListImmediate& RHICmdList, FTexture2DRHIRef RenderTargetTexture, const FSceneView *InView) {
    
    FIRCameraModule::Get().TriggerRender(RHICmdList, RenderTargetTexture, InView);
}


bool FThermalViewExtension::IsActiveThisFrame(FViewport* InViewport) const
{
    return true; // Always active
}