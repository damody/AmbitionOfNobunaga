// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class AmbitionOfNobunaga : ModuleRules
{
	public AmbitionOfNobunaga(TargetInfo Target)
	{
        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore",
            "Paper2D", "AIModule",
            "UMG", "RHI",
            "RenderCore", "SlateCore", "HeadMountedDisplay",
            "DA_RTS"});
        if (UEBuildConfiguration.bBuildEditor)
        {
            PublicDependencyModuleNames.AddRange(new string[] { "UnrealEd", "DA_RTS_Editor" });
        }
        Definitions.AddRange(new string[] { "_USE_MATH_DEFINES" });
    }
}
