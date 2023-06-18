// RolePlay Game, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RPGHealthComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnDeath);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnHealthChanged, float);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class RPG_SYSTEM_API URPGHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	URPGHealthComponent();

	float GetHealth() const {
		return Health;
	}

	UFUNCTION(BlueprintCallable)
	bool IsDead() const;

	FOnDeath OnDeath;
	FOnHealthChanged OnHealthChanged;

	void OnHeal();

	void SetHealth(float NewHealth);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (ClampMin = "0.0", ClampMax = "1000.0"))
	float MaxHealth = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float Heal = 30.0f;

	virtual void BeginPlay() override;

private:
	float Health = 0.0f;

	UFUNCTION()
	void OnTakeAnyDamage(AActor* DamageActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);
};