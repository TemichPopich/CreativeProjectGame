// RolePlay Game, All Rights Reserved

#include "Components/RPGWeaponComponent.h"
#include "GameFramework/Character.h"
#include "Weapon/RPGBaseWeapon.h"

URPGWeaponComponent::URPGWeaponComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void URPGWeaponComponent::BeginPlay()
{
    Super::BeginPlay();

    SpawnWeapon();
}

void URPGWeaponComponent::SpawnWeapon()
{
    if (!GetWorld())
    {
        return;
    }

    ACharacter *Character = Cast<ACharacter>(GetOwner());

    if (!Character)
    {
        return;
    }

    CurrentWeapon = GetWorld()->SpawnActor<ARPGBaseWeapon>(WeaponClass);

    if (!CurrentWeapon)
    {
        return;
    }

    FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, false);
    CurrentWeapon->AttachToComponent(Character->GetMesh(), AttachmentRules, "WeaponSocket");
    CurrentWeapon->SetOwner(GetOwner());
}

void URPGWeaponComponent::StartSwordAttack()
{
    if (!CurrentWeapon)
    {
        return;
    }
    CurrentWeapon->IsAttacking = true;
}

void URPGWeaponComponent::StopSwordAttack()
{
    if (!CurrentWeapon)
    {
        return;
    }
    CurrentWeapon->IsAttacking = false;
}

void URPGWeaponComponent::SwordUpgrade()
{
    CurrentWeapon->SwordDamage += 10.0f;
}
