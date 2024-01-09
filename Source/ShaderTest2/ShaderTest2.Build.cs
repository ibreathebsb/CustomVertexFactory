// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ShaderTest2 : ModuleRules
{
	public ShaderTest2(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "RenderCore", "RHI" });

		PrivateDependencyModuleNames.AddRange(new string[] {  });

        OptimizeCode = CodeOptimization.Never;



        // Uncomment if you are using Slate UI
        // PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

        // Uncomment if you are using online features
        // PrivateDependencyModuleNames.Add("OnlineSubsystem");

        // To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
    }
}
