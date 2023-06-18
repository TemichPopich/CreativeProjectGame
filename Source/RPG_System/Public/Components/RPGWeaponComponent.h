// RolePlay Game, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RPGWeaponComponent.generated.h"


class ARPGBaseWeapon;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RPG_SYSTEM_API URPGWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	URPGWeaponComponent();

	void StartSwordAttack();
	void StopSwordAttack();
	void SwordUpgrade();

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TSubclassOf<ARPGBaseWeapon> WeaponClass;

	virtual void BeginPlay() override;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	ARPGBaseWeapon* CurrentWeapon = nullptr;

	void SpawnWeapon();

};
