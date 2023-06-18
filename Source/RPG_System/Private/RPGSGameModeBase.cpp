// RolePlay Game, All Rights Reserved

#include "RPGSGameModeBase.h"
#include "RPGBaseCharacter.h"
#include "RPGPlayerController.h"

ARPGSGameModeBase::ARPGSGameModeBase()
{
    DefaultPawnClass = ARPGBaseCharacter::StaticClass();
    PlayerControllerClass = ARPGPlayerController::StaticClass();
}