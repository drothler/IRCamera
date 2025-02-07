#pragma once

class IRCAMERA_API FThermalRenderer
{
public:
	FThermalRenderer();

	void RenderThermalScene(FSceneView& InView, FRHICommandList& RHICmdList);
	
};