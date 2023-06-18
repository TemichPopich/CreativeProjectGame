// RolePlay Game, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RPGBaseCharacter.h"
#include "RPGBaseWeapon.generated.h"

class USkeletalMeshComponent;

UCLASS()
class RPG_SYSTEM_API ARPGBaseWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	ARPGBaseWeapon();

	UFUNCTION()
	void SwordAttack(class AActor* OverlaptedActor, class AActor* OtherActor);

	UPROPERTY(VisibleAnywhere, Category = "Attack")
	bool IsAttacking = false;

	UPROPERTY(VisibleAnywhere, Category = "Attack")
	float SwordDamage = 20.0f;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	USkeletalMeshComponent* WeaponMesh;

	virtual void BeginPlay() override;

};
