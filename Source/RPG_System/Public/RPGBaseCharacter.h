// RolePlay Game, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "RPGBaseCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class URPGHealthComponent;
class UTextRenderComponent;
class URPGWeaponComponent;
class ARPG_AttackSphere;
class URolePlayComponent;

UCLASS()
class RPG_SYSTEM_API ARPGBaseCharacter : public ACharacter
{
	GENERATED_BODY()


public:
	// Sets default values for this character's properties
	ARPGBaseCharacter(const FObjectInitializer& ObjInit);


protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
		UCameraComponent* CameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
		USpringArmComponent* SpringArmComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
		URPGHealthComponent* HealthComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
		URolePlayComponent* RolePlayComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
		UTextRenderComponent* HealthTextComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
		UTextRenderComponent* ManaTextComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
		URPGWeaponComponent* WeaponComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
		TSubclassOf<ARPG_AttackSphere> AttackSphereClass;

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
		UAnimMontage* DeathAnimMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
		UAnimMontage* SwordAttackMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
		UAnimMontage* MagicAttackMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
		UAnimMontage* GetHitMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
		UAnimMontage* UpLevelMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
		UAnimMontage* HealAnimMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
		FVector2D LandedDamageVelosity = FVector2D(900.0f, 1200.0f);

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
		FVector2D LandedDamage = FVector2D(10.0f, 100.0f);

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		float TraceMaxDistance = 1500.0f;

	UPROPERTY(VisibleAnywhere)
		bool SAttack = true;

	UPROPERTY(VisibleAnywhere)
		bool MAttack = false;

	UPROPERTY(VisibleAnywhere)
		float Mana;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Mana")
		float MaxMana = 80.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Mana")
		bool AutoRecovery = true;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Mana")
		float ManaUpdateTime = 1.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Mana")
		float ManaDelay = 5.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Mana")
		float ManaModifier = 3.0f;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable, Category = "Movement")
	bool IsRunning() const;

	void ChangeXP(int Value);

	void ChangeMana(float Value);

private:
	FTimerHandle ManaTimerHandle;

	void ManaUpdate();

	bool IsMovingForward = false;
	bool WantsToRTun = false;

	void MoveForwardBack(float Amount);
	void MoveLeftRight(float Amount);

	void OnStartRunning();
	void OnStopRunning();

	void OnDeath();
	void OnHealthChanged(float Health);
	void Healing();

	void LevelUp();

	UFUNCTION()
	void OnGroundLanded(const FHitResult& Hit);

	void MagicUpgrade();
	void SwordUpgrade();

	void ChangeAttackTypeOnSword();
	void ChangeAttackTypeOnMagic();

	void Attack();
	void MagicAttack();
	void MakeShot();
};
