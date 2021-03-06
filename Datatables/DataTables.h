// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/DataTable.h"
#include "DataTables.generated.h"

UENUM(BlueprintType)
enum class  EPosition : uint8 {
	HEAD,
	CHEST,
	LEFT_LEG,
	RIGHT_LEG,
	LEFT_SHOULDER,
	RIGHT_SHOULDER,
	LEFT_ARM,
	RIGHT_ARM,
	LEFT_HAND,
	RIGHT_HAND,
	BOTH_HANDS
};

USTRUCT(BlueprintType)
struct FLoadout : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loadout")
		int32 characterID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loadout")
		float maxHealth = 2000;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loadout")
		float maxSpeed = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loadout")
		TArray<int32> abilityIDs;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loadout")
		TMap<EPosition, int32> equippedWeapons;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loadout")
		TArray<int32> equippedArmour;
};

UENUM(BlueprintType)
enum class  EWeaponType : uint8 {
	NORMAL,
	AMMO,
	HEAT
};

USTRUCT(BlueprintType)
struct FWeaponSpecification : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Specification")
		int itemSpecificationID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Specification")
		EWeaponType weaponType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Specification")
		float useRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Specification")
		float healthChange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Specification")
		float range;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Specification")
		bool heals;

	//This array holds skeletal meshes for weapons 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Guns")
		FString gunMesh;

	//This holds scale for weapon meshes
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Guns")
		FVector gunScale;

	//This holds location for weapon meshes relative to the gun
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Guns")
		FVector relativeGunLocation;

	//This holds location for gun muzzle to determine location of muzzle flash and smoke
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Guns")
		FVector relativeMuzzleLocation;

	//This holds rotation for weapon meshes
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Guns")
		FRotator relativeGunRotations;
};

USTRUCT(BlueprintType)
struct FAmmoWeaponSpecification : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Specification")
		int weaponSpecificationID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Specification")
		float maxAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Specification")
		float reloadSpeed;
};

USTRUCT(BlueprintType)
struct FHeatWeaponSpecification : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Specification")
		int weaponSpecificationID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Specification")
		float maxHeat;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Specification")
		float heatGenerated;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Specification")
		float passiveHeatLoss;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Specification")
		float overheatCooldown;
};

UENUM(BlueprintType)
enum class  EAbilityType : uint8 {
	SINGLE_TARGET,
	AOE
};

USTRUCT(BlueprintType)
struct FAbilitySpecification : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability Specification")
		FString abilityName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability Specification")
		float abilityCooldown;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability Specification")
		EAbilityType abilityType;
};

USTRUCT(BlueprintType)
struct FArmourSpecification : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Armour Specification")
		int32 itemID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Armour Specification")
		EPosition armourPosition;
};

UENUM(BlueprintType)
enum class  EArmourType : uint8 {
	PHYSICAL,
	BLAST,
	ENERGY,
	RADIATION
};

USTRUCT(BlueprintType)
struct FArmourValue : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Armour Specification")
		int32 armourID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Armour Specification")
		EArmourType armourType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Armour Specification")
		float armourValue;
};


UENUM(BlueprintType)
enum class  EItemType : uint8 {
	NORMAL,
	WEAPON,
	ARMOUR
};

USTRUCT(BlueprintType)
struct FInventoryItem : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory Item")
		int itemID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory Item")
		int quantity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory Item")
		int quality;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory Item")
		int grade;
};

USTRUCT(BlueprintType)
struct FItemSpecification : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Specification")
		EItemType itemType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Specification")
		FText name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Specification")
		int stackLimit;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Specification")
		float weight;
};

UCLASS()
class SURVIVALGAME_API UDataTables : public UObject
{
	GENERATED_BODY()
public:
	UDataTables();

	static UDataTables* GetInstance();

	TArray<FItemSpecification*> GetItems();
	TArray<FWeaponSpecification*> GetWeapons();
	TArray<FHeatWeaponSpecification*> GetHeatWeapons();
	TArray<FAmmoWeaponSpecification*> GetAmmoWeapons();
	TArray<FArmourSpecification*> GetArmour();
	TArray<FArmourValue*> GetArmourValues();
	TArray<FLoadout*> GetLoadouts();

	UDataTable* GetItemTable() { return itemTable; }
	void SetItemTable(UDataTable* val) { itemTable = val; }

	UDataTable* GetLoadoutTable() { return loadoutTable; }
	void SetLoadoutTable(UDataTable* val) { loadoutTable = val; }

	UDataTable* GetWeaponTable() { return weaponTable; }
	void SetWeaponTable(UDataTable* val) { weaponTable = val; }

	UDataTable* GetAbilitiesTable() { return abilitiesTable; }
	void SetAbilitiesTable(UDataTable* val) { abilitiesTable = val; }

	UDataTable* GetArmourTable() { return armourTable; }
	void SetArmourTable(UDataTable* val) { armourTable = val; }

	UDataTable* GetArmourValuesTable() { return armourValuesTable; }
	void SetArmourValuesTable(UDataTable* val) { armourValuesTable = val; }

	UDataTable* GetHeatWeaponTable() { return heatWeaponTable; }
	void SetHeatWeaponTable(UDataTable* val) { heatWeaponTable = val; }

	UDataTable* GetAmmoWeaponTable() { return ammoWeaponTable; }
	void SetAmmoWeaponTable(UDataTable* val) { ammoWeaponTable = val; }
private:
	static UDataTables* INSTANCE;
	UDataTable* itemTable;
	UDataTable* weaponTable;
	UDataTable* heatWeaponTable;
	UDataTable* ammoWeaponTable;
	UDataTable* loadoutTable;
	UDataTable* abilitiesTable;
	UDataTable* armourTable;
	UDataTable* armourValuesTable;
};
