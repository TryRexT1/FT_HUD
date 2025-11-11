// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class FT_HUD : ModuleRules
{
	public FT_HUD(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput" });
	}
}
