// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Tool.h"
#include "../Datatables/DataTables.h"
#include "Weapon.generated.h"

class ASurvivalGameCharacter;
UCLASS()
class SURVIVALGAME_API UWeapon : public UTool
{
	GENERATED_BODY()

private:
	FWeaponSpecification* weaponSpecification;

	virtual bool CanAttack();
	virtual void FireWeapon(ASurvivalGameCharacter* target);
public:
	static UWeapon* CreateWeapon(int32 itemID, FItemSpecification weaponSpecification);

	FWeaponSpecification* GetWeaponSpecification() { return weaponSpecification; }

	void SetWeaponSpecification(FWeaponSpecification* val) { weaponSpecification = val; }
	void AttackTarget(ASurvivalGameCharacter* target);
	
};
