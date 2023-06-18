// RolePlay Game, All Rights Reserved

#include "Weapon/RPGBaseWeapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/World.h"
#include "GameFramework/Character.h"

DEFINE_LOG_CATEGORY_STATIC(LogBaseWeapon, All, All);

ARPGBaseWeapon::ARPGBaseWeapon()
{
    PrimaryActorTick.bCanEverTick = false;

    WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>("WeaponMesh");
    SetRootComponent(WeaponMesh);

    OnActorBeginOverlap.AddDynamic(this, &ARPGBaseWeapon::SwordAttack);
}

void ARPGBaseWeapon::SwordAttack(AActor *OverlaptedActor, AActor *OtherActor)
{
    if (!GetWorld())
    {
        return;
    }

    const auto Player = Cast<ACharacter>(GetOwner());

    if (!Player)
    {
        return;
    }

    if (OtherActor == Player)
    {
        IsAttacking = false;
        return;
    }

    if (OtherActor && OtherActor != this)
    {
        ARPGBaseCharacter *Character = Cast<ARPGBaseCharacter>(OtherActor);
        if (Character && IsAttacking)
        {
            IsAttacking = false;
            Character->TakeDamage(SwordDamage, {}, nullptr, this);
        }
    }
}

void ARPGBaseWeapon::BeginPlay()
{
    Super::BeginPlay();
}
