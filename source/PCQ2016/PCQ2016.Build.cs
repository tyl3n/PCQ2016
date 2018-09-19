// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class PCQ2016 : ModuleRules
{
	public PCQ2016(ReadOnlyTargetRules Target) : base (Target)
	{
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore" });
	}
}
