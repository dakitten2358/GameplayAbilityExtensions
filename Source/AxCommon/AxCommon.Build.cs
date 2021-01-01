// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class AxCommon : ModuleRules
{
    protected bool UseEnhancedInput = true;

	public AxCommon(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.Add("Core");
        PublicDependencyModuleNames.Add("CoreUObject");
        PublicDependencyModuleNames.Add("Engine");
        PublicDependencyModuleNames.Add("GameplayAbilities");
        PublicDependencyModuleNames.Add("GameplayTags");
        PublicDependencyModuleNames.Add("GameplayTasks");
        PublicDependencyModuleNames.Add("AIModule");

        if (Target.bBuildEditor == true)
        {
            PrivateDependencyModuleNames.Add("UnrealEd");
            PrivateDependencyModuleNames.Add("Slate");
        }

        if (Target.bBuildDeveloperTools || IsDevBuild(Target))
        {
            PrivateDependencyModuleNames.Add("GameplayDebugger");
            PublicDefinitions.Add("WITH_GAMEPLAY_DEBUGGER=1");
        }
        else
        {
            PublicDefinitions.Add("WITH_GAMEPLAY_DEBUGGER=0");
        }

        if (UseEnhancedInput)
        {
            PrivateDefinitions.Add("WITH_ENHANCED_INPUT=1");
            PublicDependencyModuleNames.Add("EnhancedInput");
        }
    }

    private bool IsDevBuild(ReadOnlyTargetRules Target)
    {
        bool bIsShippingBuild = Target.Configuration == UnrealTargetConfiguration.Shipping;
        bool bIsTestBuild = Target.Configuration == UnrealTargetConfiguration.Test;
        return (!bIsShippingBuild && !bIsTestBuild);
    }
}
