using UnrealBuildTool;

public class POTS_Polygon : ModuleRules
{
    public POTS_Polygon(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[]
        {
            "Core",
            "CoreUObject",
            "Engine",
            "NetCore",
            "EnhancedInput"
        });
    }
}
