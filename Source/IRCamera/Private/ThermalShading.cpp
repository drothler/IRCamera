#include "ThermalShading.h"
#include "ThermalRendering.h"

#include "MeshMaterialShader.h"
#include "Private/ScenePrivate.h"

IMPLEMENT_MATERIAL_SHADER_TYPE(,ThermalVS, TEXT("/Plugin/IRCamera/Private/ThermalShading.usf"), TEXT("MainVS"), SF_Vertex);
IMPLEMENT_MATERIAL_SHADER_TYPE(,ThermalPS,TEXT("/Plugin/IRCamera/Private/ThermalShading.usf"),TEXT("MainPS"),SF_Pixel);
IMPLEMENT_SHADERPIPELINE_TYPE_VSPS(ThermalOutputPipeline, ThermalVS, ThermalPS, true);
IMPLEMENT_GLOBAL_SHADER_PARAMETER_STRUCT(FThermalParams, "FThermalParams");

static FORCEINLINE bool UseShaderPipelines(ERHIFeatureLevel::Type InFeatureLevel)
{
	static const auto* CVar = IConsoleManager::Get().FindTConsoleVariableDataInt(TEXT("r.ShaderPipelines"));
	return RHISupportsShaderPipelines(GShaderPlatformForFeatureLevel[InFeatureLevel]) && CVar && CVar->GetValueOnAnyThread() != 0;
}

void GetThermalPassShaders(const FMaterial& Material, FVertexFactoryType* VertexFactoryType, ERHIFeatureLevel::Type FeatureLevel, TShaderRef<ThermalVS>& VertexShader, TShaderRef<ThermalPS>& PixelShader, FShaderPipelineRef& ShaderPipeline)
{
	ShaderPipeline = UseShaderPipelines(FeatureLevel) ? Material.GetShaderPipeline(&ThermalOutputPipeline, VertexFactoryType, false) : FShaderPipelineRef();

	if (ShaderPipeline.IsValid())
	{
		VertexShader = ShaderPipeline.GetShader<ThermalVS>();
		PixelShader = ShaderPipeline.GetShader<ThermalPS>();
	}
}

FThermalPassMeshProcessor::FThermalPassMeshProcessor(
	const FScene* Scene, 
	const FSceneView* InView, 
	FMeshPassDrawListContext* InDrawListContext,
	FRHIUniformBuffer* InUniformBuffer)
	: FMeshPassProcessor(Scene, Scene ? Scene->GetFeatureLevel() : GMaxRHIFeatureLevel, InView, InDrawListContext),
	PassDrawRenderState(*InView, InUniformBuffer)
{
	PassDrawRenderState.SetDepthStencilState(TStaticDepthStencilState<false, CF_Always>::GetRHI());
	PassDrawRenderState.SetBlendState(TStaticBlendState<>::GetRHI());
	PassDrawRenderState.SetViewUniformBuffer(Scene->UniformBuffers.ViewUniformBuffer);
	PassDrawRenderState.SetInstancedViewUniformBuffer(Scene->UniformBuffers.InstancedViewUniformBuffer);
}

void FThermalPassMeshProcessor::Process(const FMeshBatch& MeshBatch, uint64 BatchElementMask, int32 StaticMeshId, const FPrimitiveSceneProxy* PrimitiveSceneProxy, const FMaterialRenderProxy& MaterialRenderProxy, const FMaterial& MaterialResource)
{
	const FVertexFactory* VertexFactory = MeshBatch.VertexFactory;

	TMeshProcessorShaders<ThermalVS, FMeshMaterialShader, FMeshMaterialShader, ThermalPS> Shaders;

	Shaders.VertexShader = MaterialResource.GetShader<ThermalVS>(VertexFactory->GetType());
	Shaders.PixelShader = MaterialResource.GetShader<ThermalPS>(VertexFactory->GetType());

	const FMeshDrawingPolicyOverrideSettings OverrideSettings = ComputeMeshOverrideSettings(MeshBatch);
	ERasterizerFillMode MeshFillMode = ComputeMeshFillMode(MeshBatch, MaterialResource, OverrideSettings);
	ERasterizerCullMode MeshCullMode = CM_None;

	FThermalShaderElementData ShaderElementData;
	ShaderElementData.InitializeMeshMaterialData(ViewIfDynamicMeshCommand, PrimitiveSceneProxy, MeshBatch, StaticMeshId, false);

	FMeshDrawCommandSortKey SortKey = {};

	BuildMeshDrawCommands(
			MeshBatch,
			BatchElementMask,
			PrimitiveSceneProxy,
			MaterialRenderProxy,
			MaterialResource,
			PassDrawRenderState,
			Shaders,
			MeshFillMode,
			MeshCullMode,
			SortKey,
			EMeshPassFeatures::Default,
			ShaderElementData);
}


void FThermalPassMeshProcessor::AddMeshBatch(const FMeshBatch& RESTRICT MeshBatch, uint64 BatchElementMask, const FPrimitiveSceneProxy* RESTRICT PrimitiveSceneProxy, int32 StaticMeshId)
{
	const FMaterialRenderProxy* FallbackMaterialRenderProxyPtr = nullptr;
	const FMaterial& Material = MeshBatch.MaterialRenderProxy->GetMaterialWithFallback(FeatureLevel, FallbackMaterialRenderProxyPtr);
	const FMaterialRenderProxy& MaterialRenderProxy = FallbackMaterialRenderProxyPtr ? *FallbackMaterialRenderProxyPtr : *MeshBatch.MaterialRenderProxy;

	
	if (MeshBatch.bUseForMaterial
		&& (Material.GetBlendMode() == BLEND_Opaque || Material.IsMasked())
		&& (!PrimitiveSceneProxy || PrimitiveSceneProxy->ShouldRenderInMainPass()))
	{
		Process(MeshBatch, BatchElementMask, StaticMeshId, PrimitiveSceneProxy, MaterialRenderProxy, Material);
	}
}


FThermalRenderer::FThermalRenderer()
{
	UE_LOG(LogTemp, Display, TEXT("ThermalRenderer created"));
}

void FThermalRenderer::RenderThermalScene(FSceneView& InView, FRHICommandList& RHICmdList)
{
	UE_LOG(LogTemp, Display, TEXT("Calling Thermal Render Thread"));

	FThermalParams ThermalParams;
	
	FThermalUniformBufferRef PassUniformBuffer = FThermalUniformBufferRef::CreateUniformBufferImmediate(ThermalParams, UniformBuffer_SingleFrame);

	DrawDynamicMeshPass(InView, RHICmdList, [View = InView, PassUniformBuffer](FDynamicPassMeshDrawListContext* DynamicMeshPassContext)
	{
		FScene* Scene = View.Family->Scene->GetRenderScene();
		const FSceneView* SceneViewPtr = &View;
		FThermalPassMeshProcessor MeshProcessor{Scene, SceneViewPtr, DynamicMeshPassContext, PassUniformBuffer};

		constexpr uint64 DefaultBatchElementMask = ~0ull;

		
		for (const FPrimitiveSceneInfo* PrimitiveSceneInfo : Scene->Primitives)
		{
			if (!PrimitiveSceneInfo || !PrimitiveSceneInfo->Proxy)
			{
				continue;
			}


			const int32 NumStaticMeshes = PrimitiveSceneInfo->StaticMeshRelevances.Num();
			for(int32 MeshIndex = 0;MeshIndex < NumStaticMeshes;MeshIndex++)
			{
				const FStaticMeshBatchRelevance& StaticMeshRelevance = PrimitiveSceneInfo->StaticMeshRelevances[MeshIndex];
				const FStaticMeshBatch& StaticMesh = PrimitiveSceneInfo->StaticMeshes[MeshIndex];
			
				//UE_LOG(LogTemp, Log, TEXT("Mesh Batch Found: %s"), *StaticMesh.MaterialRenderProxy->GetFriendlyName());
				// Add the mesh batch to the mesh processor
				MeshProcessor.AddMeshBatch(StaticMesh, DefaultBatchElementMask, PrimitiveSceneInfo->Proxy);
				
			}
		}
	});
	
}
