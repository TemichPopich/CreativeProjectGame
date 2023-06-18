// RolePlay Game, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RPG_AttackSphere.generated.h"

class USphereComponent;
class UProjectileMovementComponent;

UCLASS()
class RPG_SYSTEM_API ARPG_AttackSphere : public AActor
{
	GENERATED_BODY()
	
public:	
	ARPG_AttackSphere();

	void SetShotDirection(const FVector& Direction) {
		ShotDirection = Direction;
	}

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleDefaultsOnly, Category = "Weapon")
	USphereComponent* CollisionComponent;

	UPROPERTY(VisibleDefaultsOnly, Category = "Weapon")
	UProjectileMovementComponent* MovementComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	float DamageRadius = 200.0f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	float DamageAmount = 30.0f;

private:
	UFUNCTION()
	void OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	FVector ShotDirection;
};
