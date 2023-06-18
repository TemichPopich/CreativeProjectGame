// RolePlay Game, All Rights Reserved

using UnrealBuildTool;
using System.Collections.Generic;

public class RPG_SystemEditorTarget : TargetRules
{
	public RPG_SystemEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V2;

		ExtraModuleNames.AddRange( new string[] { "RPG_System" } );
	}
}
