// RolePlay Game, All Rights Reserved

#include "Weapon/RPG_AttackSphere.h"
#include "Components/SphereComponent.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"

ARPG_AttackSphere::ARPG_AttackSphere()
{
    PrimaryActorTick.bCanEverTick = false;

    CollisionComponent = CreateDefaultSubobject<USphereComponent>("CollisionComponent");
    CollisionComponent->InitSphereRadius(5.0f);
    CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    SetRootComponent(CollisionComponent);

    MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("MovementComponent");
}

void ARPG_AttackSphere::BeginPlay()
{
    Super::BeginPlay();

    check(MovementComponent);

    CollisionComponent->OnComponentHit.AddDynamic(this, &ARPG_AttackSphere::OnProjectileHit);
    MovementComponent->Velocity = ShotDirection * MovementComponent->InitialSpeed;
    SetLifeSpan(5.0f);
}

void ARPG_AttackSphere::OnProjectileHit(UPrimitiveComponent *HitComponent, AActor *OtherActor,
                                        UPrimitiveComponent *OtherComp, FVector NormalImpulse, const FHitResult &Hit)
{
    if (!GetWorld())
    {
        return;
    }

    MovementComponent->StopMovementImmediately();

    UGameplayStatics::ApplyRadialDamage(GetWorld(), DamageAmount, GetActorLocation(), DamageRadius,
                                        UDamageType::StaticClass(), {GetOwner()}, this, nullptr, false);
    DrawDebugSphere(GetWorld(), GetActorLocation(), DamageRadius, 32, FColor::Red, false, 3.0f);

    Destroy();
}
