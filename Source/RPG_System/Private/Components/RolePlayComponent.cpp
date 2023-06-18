// RolePlay Game, All Rights Reserved

#include "Components/RolePlayComponent.h"
#include "Dev/RPGXPActor.h"

DEFINE_LOG_CATEGORY_STATIC(LogRP, All, All);

URolePlayComponent::URolePlayComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void URolePlayComponent::SpawnXP()
{
    if (!GetWorld())
    {
        return;
    }
    const FTransform SpawnTransform(FRotator::ZeroRotator, GetOwner()->GetActorLocation());
    XPActor = GetWorld()->SpawnActor<ARPGXPActor>(XPActorClass, SpawnTransform);
}

void URolePlayComponent::SetXP(int NewValue)
{
    if (NewValue >= 100)
    {
        CurrentXP = NewValue - 100;
        LevelUp();
        return;
    }
    CurrentXP = NewValue;
}

int URolePlayComponent::GetXP()
{
    return CurrentXP;
}

void URolePlayComponent::OnXPChanged()
{
    GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("+ 60 XP"), true, FVector2D(2.0f, 2.0f));
}

void URolePlayComponent::BeginPlay()
{
    Super::BeginPlay();
}

void URolePlayComponent::LevelUp()
{
    Level += 1;
    CanUpScills = true;
    OnLevelUp.Broadcast();
}
