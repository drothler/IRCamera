#include "ThermalShader.h"
#include "CoreMinimal.h"
#include "PixelShaderUtils.h"
#include "RHI.h"
#include "RenderResource.h"
#include "ShaderParameters.h"
#include "GlobalShader.h"
#include "Shader.h"
#include "RenderGraphBuilder.h"
#include "RenderGraphUtils.h"
#include "ShaderParameterMacros.h"
#include "ShaderParameterStruct.h"
#include "UniformBuffer.h"
#include "RHICommandList.h"
#include "Containers/DynamicRHIResourceArray.h"
#include "Runtime/RenderCore/Public/PixelShaderUtils.h"
#include "ClearQuad.h"
#include "MaterialShaderType.h"
#include "MaterialShader.h"
/** Index buffer for drawing the quad */
FIndexBufferRHIRef GIndexBufferRHI;
/** Vertex buffer for drawing the quad */
FVertexBufferRHIRef GVertexBufferRHI;


class FThermalVertexBuffer : public FVertexBuffer
{
    public:
        void InitRHI()
        {
            TResourceArray<FFilterVertex, VERTEXBUFFER_ALIGNMENT> Vertices;
            Vertices.SetNumUninitialized(6);

            Vertices[0].Position = FVector4(-1, 1, 0, 1);
            Vertices[0].UV = FVector2D(0, 0);

            Vertices[1].Position = FVector4(1, 1, 0, 1);
            Vertices[1].UV = FVector2D(1, 0);

            Vertices[2].Position = FVector4(-1, -1, 0, 1);
            Vertices[2].UV = FVector2D(0, 1);

            Vertices[3].Position = FVector4(1, -1, 0, 1);
            Vertices[3].UV = FVector2D(1, 1);

            // Create vertex buffer. Fill buffer with initial data upon creation
            FRHIResourceCreateInfo CreateInfo(&Vertices);
            GVertexBufferRHI = RHICreateVertexBuffer(Vertices.GetResourceDataSize(), BUF_Static, CreateInfo);

            // Setup index buffer
            const uint16 Indices[] = { 0, 1, 2, 2, 1, 3 };

            TResourceArray<uint16, INDEXBUFFER_ALIGNMENT> IndexBuffer;
            const uint32 NumIndices = UE_ARRAY_COUNT(Indices);
            IndexBuffer.AddUninitialized(NumIndices);
            FMemory::Memcpy(IndexBuffer.GetData(), Indices, NumIndices * sizeof(uint16));

            FRHIResourceCreateInfo CreateInfoIB(&IndexBuffer);
            GIndexBufferRHI = RHICreateIndexBuffer(sizeof(uint16), IndexBuffer.GetResourceDataSize(), BUF_Static, CreateInfoIB);
        }
};


TGlobalResource<FThermalVertexBuffer> GThermalVertexBuffer;



class FPassThroughVS: public FGlobalShader
{
    public: 
        DECLARE_GLOBAL_SHADER(FPassThroughVS);

    public: 
        static bool ShouldCompilePermutation(const FGlobalShaderPermutationParameters& Parameters)
        {
            return true;
        }

        FPassThroughVS() {}

        FPassThroughVS(const FGlobalShaderType::CompiledShaderInitializerType& Initializer): FGlobalShader(Initializer) {}
};


class FThermalPS : public FGlobalShader
{
    public:
        DECLARE_GLOBAL_SHADER(FThermalPS);
        SHADER_USE_PARAMETER_STRUCT(FThermalPS, FGlobalShader);
        BEGIN_SHADER_PARAMETER_STRUCT(FParameters, )
            /* SHADER_PARAMETER_TEXTURE(Texture2D, BaseThermalTexture)
            SHADER_PARAMETER_SAMPLER(SamplerState, Sampler) */
        END_SHADER_PARAMETER_STRUCT()

    public: 
        static bool ShouldCompilePermutation(const FGlobalShaderPermutationParameters& Parameters)
        {
            return IsFeatureLevelSupported(Parameters.Platform, ERHIFeatureLevel::SM5);
        }

        static void ModifyCompilationEnvironment(const FGlobalShaderPermutationParameters& Parameters, FShaderCompilerEnvironment& OutEnvironment)
        {
            FGlobalShader::ModifyCompilationEnvironment(Parameters, OutEnvironment);
        }

        //void SetParameters(FRHICommandList& RHICmdList, const FShaderRHIParamRef ShaderRHI, const FParameters& Parameters);
        void SetParameters(FRHICommandList& RHICmdList, const FParameters& Parameters) {
            //SetShaderParameters(RHICmdList,  this, Parameters);
        }

};

IMPLEMENT_MATERIAL_SHADER_TYPE(, FPassThroughVS, TEXT("/Plugin/IRCamera/Private/ThermalShader.usf"), TEXT("MainVS"), SF_Vertex);
//IMPLEMENT_GLOBAL_SHADER(FThermalPS, "/Plugin/IRCamera/Private/ThermalShader.usf", "MainPS", SF_Pixel);
IMPLEMENT_MATERIAL_SHADER_TYPE(, FThermalPS, TEXT("/Plugin/IRCamera/Private/ThermalShader.usf"), TEXT("MainPS"), SF_Pixel);

void ThermalShaderUtil::ExecuteRenderPass(FRHICommandListImmediate& RHICmdList) {

    QUICK_SCOPE_CYCLE_COUNTER(STAT_ShaderPlugin_PixelShader); // Used to gather CPU profiling data for the UE4 session frontend
	SCOPED_DRAW_EVENT(RHICmdList, ShaderPlugin_Pixel); // Used to profile GPU activity and add metadata to be consumed by for example RenderDoc
    
    auto ShaderMap = GetGlobalShaderMap(GMaxRHIFeatureLevel);
    TShaderMapRef<FPassThroughVS> VertexShader(ShaderMap);
    TShaderMapRef<FThermalPS> PixelShader(ShaderMap);

    FGraphicsPipelineStateInitializer GraphicsPSOInit;
    RHICmdList.ApplyCachedRenderTargets(GraphicsPSOInit);
    GraphicsPSOInit.BlendState = TStaticBlendState<>::GetRHI();
    GraphicsPSOInit.RasterizerState = TStaticRasterizerState<>::GetRHI();
    GraphicsPSOInit.DepthStencilState = TStaticDepthStencilState<false, CF_Always>::GetRHI();
    GraphicsPSOInit.BoundShaderState.VertexDeclarationRHI = GFilterVertexDeclaration.VertexDeclarationRHI;
    GraphicsPSOInit.BoundShaderState.VertexShaderRHI = VertexShader.GetVertexShader();
    GraphicsPSOInit.BoundShaderState.PixelShaderRHI = PixelShader.GetPixelShader();
    GraphicsPSOInit.PrimitiveType = PT_TriangleStrip;
    SetGraphicsPipelineState(RHICmdList, GraphicsPSOInit);
    FThermalPS::FParameters PassParameters;
    SetShaderParameters(RHICmdList, PixelShader, PixelShader.GetPixelShader(), PassParameters);
    RHICmdList.SetStreamSource(0, GVertexBufferRHI, 0);
    RHICmdList.DrawIndexedPrimitive(
				GIndexBufferRHI,
				/*BaseVertexIndex=*/ 0,
				/*MinIndex=*/ 0,
				/*NumVertices=*/ 4,
				/*StartIndex=*/ 0,
				/*NumPrimitives=*/ 2,
				/*NumInstances=*/ 1
    );
}

void ThermalShaderUtil::ForceRedScreen(FRHICommandListImmediate& RHICmdList, FTexture2DRHIRef RenderTargetTexture, const FSceneView *InView)
{
    // Set the render target
    /* FRHIRenderPassInfo RenderPassInfo(RenderTargetTexture, ERenderTargetActions::Clear_Store);
    RHICmdList.BeginRenderPass(RenderPassInfo, TEXT("ForceRedScreen")); */

    // Set the viewport
    /* RHICmdList.SetViewport(0, 0, 0, RenderTargetTexture->GetSizeX(), RenderTargetTexture->GetSizeY(), 1.0f); */

    auto ShaderMap = GetGlobalShaderMap(GMaxRHIFeatureLevel);
    TShaderMapRef<FPassThroughVS> VertexShader(ShaderMap);
    TShaderMapRef<FThermalPS> PixelShader(ShaderMap);

    // Set the graphics pipeline state
    FGraphicsPipelineStateInitializer GraphicsPSOInit;
    RHICmdList.ApplyCachedRenderTargets(GraphicsPSOInit);
    GraphicsPSOInit.BlendState = TStaticBlendState<>::GetRHI();
    GraphicsPSOInit.RasterizerState = TStaticRasterizerState<>::GetRHI();
    GraphicsPSOInit.DepthStencilState = TStaticDepthStencilState<false, CF_Always>::GetRHI();
    GraphicsPSOInit.BoundShaderState.VertexDeclarationRHI = GFilterVertexDeclaration.VertexDeclarationRHI;
    GraphicsPSOInit.BoundShaderState.VertexShaderRHI = VertexShader.GetVertexShader();
    GraphicsPSOInit.BoundShaderState.PixelShaderRHI = PixelShader.GetPixelShader();
    GraphicsPSOInit.PrimitiveType = PT_TriangleList;
    SetGraphicsPipelineState(RHICmdList, GraphicsPSOInit);
    const FIntPoint ViewSize = InView->UnconstrainedViewRect.Size();
	/* FDrawRectangleParameters Parameters;
	Parameters.PosScaleBias = FVector4(ViewSize.X, ViewSize.Y, 0, 0);
	Parameters.UVScaleBias = FVector4(1.0f, 1.0f, 0.0f, 0.0f);
	Parameters.InvTargetSizeAndTextureSize = FVector4(
			1.0f / ViewSize.X, 1.0f / ViewSize.Y,
			1.0f, 1.0f);

	SetUniformBufferParameterImmediate(RHICmdList, VertexShader.GetVertexShader(), VertexShader->GetUniformBufferParameter<FDrawRectangleParameters>(), Parameters); */

    // Draw a full-screen quad
    RHICmdList.SetStreamSource(0, GVertexBufferRHI, 0);
    RHICmdList.DrawIndexedPrimitive(
				GIndexBufferRHI,
				/*BaseVertexIndex=*/ 0,
				/*MinIndex=*/ 0,
				/*NumVertices=*/ 4,
				/*StartIndex=*/ 0,
				/*NumPrimitives=*/ 2,
				/*NumInstances=*/ 1
    );

}