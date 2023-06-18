

#include "Components/RPGHealthComponent.h"
#include "GameFramework/Actor.h"

DEFINE_LOG_CATEGORY_STATIC(LogHealthComponent, All, All)

URPGHealthComponent::URPGHealthComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

bool URPGHealthComponent::IsDead() const
{
    return Health <= 0.0f;
}

void URPGHealthComponent::OnHeal()
{
    SetHealth(Health + Heal);
}

void URPGHealthComponent::SetHealth(float NewHealth)
{
    Health = FMath::Clamp(NewHealth, 0.0f, MaxHealth);
    OnHealthChanged.Broadcast(Health);
}

// Called when the game starts
void URPGHealthComponent::BeginPlay()
{
    Super::BeginPlay();

    SetHealth(MaxHealth);

    OnHealthChanged.Broadcast(Health);

    AActor *ComponentOwner = GetOwner();
    if (ComponentOwner)
    {
        ComponentOwner->OnTakeAnyDamage.AddDynamic(this, &URPGHealthComponent::OnTakeAnyDamage);
    }
}

void URPGHealthComponent::OnTakeAnyDamage(AActor *DamageActor, float Damage, const UDamageType *DamageType,
                                          AController *InstigatedBy, AActor *DamageCauser)
{
    if (Damage <= 0.0f || IsDead())
    {
        return;
    }
    SetHealth(Health - Damage);

    if (IsDead())
    {
        OnDeath.Broadcast();
    }
}
