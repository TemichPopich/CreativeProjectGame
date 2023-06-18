// RolePlay Game, All Rights Reserved

using UnrealBuildTool;
using System.Collections.Generic;

public class RPG_SystemTarget : TargetRules
{
	public RPG_SystemTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V2;

		ExtraModuleNames.AddRange( new string[] { "RPG_System" } );
	}
}
