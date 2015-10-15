// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class AmbitionOfNobunaga : ModuleRules
{
	public AmbitionOfNobunaga(TargetInfo Target)
	{
        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore",
            "Paper2D",
            "UMG", "RHI",
            "RenderCore", "SlateCore", "HeadMountedDisplay" });
	}
}
