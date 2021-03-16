// (C) 2021 Jay Murray

using System.IO;
using UnrealBuildTool;

public class AxWeapons : ModuleRules
{
	public AxWeapons(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.AddRange(
			new string[] 
			{
                Path.Combine(ModuleDirectory, "Public")

			}
			);
				
		
		PrivateIncludePaths.AddRange(
			new string[] 
			{
				
			}
			);

        PublicDependencyModuleNames.Add("Core");
        PublicDependencyModuleNames.Add("GameplayAbilities");
        PublicDependencyModuleNames.Add("GameplayTags");
        PublicDependencyModuleNames.Add("GameplayTasks");
        PublicDependencyModuleNames.Add("AxCommon");
        PublicDependencyModuleNames.Add("Niagara");
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"CoreUObject",
				"Engine",
			}
			);
		
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				
			}
			);
	}
}
