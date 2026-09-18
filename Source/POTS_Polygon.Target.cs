using UnrealBuildTool;
using System.Collections.Generic;

public class POTS_PolygonTarget : TargetRules
{
    public POTS_PolygonTarget(TargetInfo Target) : base(Target)
    {
        Type = TargetType.Game;
        DefaultBuildSettings = BuildSettingsVersion.V6;
        IncludeOrderVersion = EngineIncludeOrderVersion.Latest;
        ExtraModuleNames.AddRange(new string[] { "POTS_Polygon" });
    }
}
