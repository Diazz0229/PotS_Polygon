using UnrealBuildTool;
using System.Collections.Generic;

public class POTS_PolygonEditorTarget : TargetRules
{
    public POTS_PolygonEditorTarget(TargetInfo Target) : base(Target)
    {
        Type = TargetType.Editor;
        DefaultBuildSettings = BuildSettingsVersion.V6;
        IncludeOrderVersion = EngineIncludeOrderVersion.Latest;
        ExtraModuleNames.AddRange(new string[] { "POTS_Polygon" });
    }
}
