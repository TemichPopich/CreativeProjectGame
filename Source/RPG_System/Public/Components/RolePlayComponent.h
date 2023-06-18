// RolePlay Game, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RolePlayComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnLevelUp)

class ARPGXPActor;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RPG_SYSTEM_API URolePlayComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	URolePlayComponent();

	UPROPERTY(EditDefaultsOnly, Category = "Skills")
	int Level = 1;

	UPROPERTY(EditDefaultsOnly, Category = "Skills")
	bool CanUpScills = false;

	UPROPERTY(EditDefaultsOnly, Category = "Skills")
	int SwordSkill = 1;

	UPROPERTY(EditDefaultsOnly, Category = "Skills")
	int MagicSkill = 0;

	UPROPERTY(EditDefaultsOnly, Category = "Skills")
	int XP = 60;

	FOnLevelUp OnLevelUp;

	void SpawnXP();

	void SetXP(int NewValue);

	int GetXP();

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Scill")
		TSubclassOf<ARPGXPActor> XPActorClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Skill")
		ARPGXPActor* XPActor = nullptr;

	void OnXPChanged();

	virtual void BeginPlay() override;	

private: 
	int CurrentXP = 0;

	void LevelUp();
};
