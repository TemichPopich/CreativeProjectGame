// RolePlay Game, All Rights Reserved

#include "Components/RPGCharacterMovementComponent.h"
#include "RPGBaseCharacter.h"

float URPGCharacterMovementComponent::GetMaxSpeed() const
{
    const float MaxSpeed = Super::GetMaxSpeed();
    const ARPGBaseCharacter *Player = Cast<ARPGBaseCharacter>(GetPawnOwner());
    return Player && Player->IsRunning() ? MaxSpeed * RunModifier : MaxSpeed;
}
