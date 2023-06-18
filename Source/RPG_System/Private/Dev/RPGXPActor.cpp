// RolePlay Game, All Rights Reserved

#include "Dev/RPGXPActor.h"
#include "Components/SphereComponent.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Character.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY_STATIC(LogActorXP, All, All);

ARPGXPActor::ARPGXPActor()
{
    PrimaryActorTick.bCanEverTick = false;

    CollisionComponent = CreateDefaultSubobject<USphereComponent>("CollisionComponent");
    CollisionComponent->InitSphereRadius(20.0f);
    CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    SetRootComponent(CollisionComponent);
}

void ARPGXPActor::BeginPlay()
{
    Super::BeginPlay();

    check(CollisionComponent);
    CollisionComponent->OnComponentHit.AddDynamic(this, &ARPGXPActor::OnComponentXPHit);
    SetLifeSpan(15.0f);
}

void ARPGXPActor::OnComponentXPHit(UPrimitiveComponent *HitComponent, AActor *OtherActor,
                                   UPrimitiveComponent *OtherComp, FVector NormalImpulse, const FHitResult &Hit)
{
    UE_LOG(LogActorXP, Display, TEXT("Hello Motherfucker"));

    ARPGBaseCharacter *Character = Cast<ARPGBaseCharacter>(OtherActor);

    Character->ChangeXP(60);

    Destroy();
}