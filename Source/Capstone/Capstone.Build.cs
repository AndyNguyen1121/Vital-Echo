// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Capstone : ModuleRules
{
	public Capstone(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"GameplayAbilities",
			"GameplayTasks",
			"GameplayTags",
			"UMG",
			"MotionWarping",
			"AIModule"
		});

		PrivateDependencyModuleNames.AddRange(new string[]
		{
			"LevelSequence",
			"MovieScene",
			"ContextualAnimation"
		});
		
		if (Target.Type == TargetType.Editor)
		{
			PrivateDependencyModuleNames.AddRange(new string[]
			{
				"AnimationBlueprintLibrary",
				"AnimationModifiers",
				"UnrealEd",
				"EngineAssetDefinitions"
			});
		}
	}
}
