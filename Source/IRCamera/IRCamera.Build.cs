// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class IRCamera : ModuleRules
{
	public IRCamera(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
	
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"RenderCore",
				"Renderer",
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
				"RHI"
				// ... add private dependencies that you statically link with here ...	
			}
			);

		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"Projects",
				// ... add private dependencies that you statically link with here ...
			}
			);

		if (Target.bBuildEditor)
		{
			PrivateDependencyModuleNames.AddRange(
				new string[] {
					"MaterialEditor"
				}
			);
		}
	}
}
