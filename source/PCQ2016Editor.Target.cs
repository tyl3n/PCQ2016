// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;
using System.Collections.Generic;

public class PCQ2016EditorTarget : TargetRules
{
	public PCQ2016EditorTarget(TargetInfo Target): base (Target)
	{
		Type = TargetType.Editor;
		ExtraModuleNames.Add("PCQ2016");
	}
}
