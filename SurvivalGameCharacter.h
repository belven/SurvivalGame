// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Datatables/DataTables.h"
#include "SurvivalGameCharacter.generated.h"

class UInputComponent;
class UInventory;
class UWeapon;
class USkillTree;
class UAbility;
class UStat;
class UGroup;
class UArmour;

UCLASS(config = Game)
class ASurvivalGameCharacter : public ACharacter
{
	GENERATED_BODY()
private:

	UPROPERTY(EditAnywhere, Category = "ID")
		int32 ID;
	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		class USkeletalMeshComponent* Mesh1P;

	/** Gun mesh: 1st person view (seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		class USkeletalMeshComponent* FP_Gun;

	/** Location on gun mesh where projectiles should spawn. */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		class USceneComponent* FP_MuzzleLocation;

	/** Gun mesh: VR view (attached to the VR controller directly, no arm, just the actual gun) */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		class USkeletalMeshComponent* VR_Gun;

	/** Location on VR gun mesh where projectiles should spawn. */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		class USceneComponent* VR_MuzzleLocation;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* FirstPersonCameraComponent;

	/** Motion controller (right hand) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UMotionControllerComponent* R_MotionController;

	/** Motion controller (left hand) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UMotionControllerComponent* L_MotionController;

	UPROPERTY(EditAnywhere, Category = "Group")
		UGroup* group;

	UPROPERTY(EditAnywhere, Category = "Name")
		TArray<UStat*> stats;

	UPROPERTY(EditAnywhere, Category = "Weapon")
		TMap<EPosition, UWeapon*> equipedWeapons;

	UPROPERTY(EditAnywhere, Category = "Armour")
		TMap<EPosition, UArmour*> armour;

	UPROPERTY(EditAnywhere, Category = "Name")
		FText characterName;

	UFUNCTION(BlueprintCallable, Category = "Stats")
		void MaximiseStats();

	UFUNCTION(BlueprintCallable, Category = "Stats")
		void AddStat(UStat* newStat);

	UFUNCTION(BlueprintCallable, Category = "Stats")
		bool IsAlive();

	UFUNCTION(BlueprintCallable, Category = "Stats")
		bool CanAttack();
public:
	static const FText healthStatName;
	ASurvivalGameCharacter();

	virtual void BeginPlay();
	void InteractWithTarget(ASurvivalGameCharacter* target);

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseLookUpRate;

	/** Gun muzzle's offset from the characters location */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		FVector GunOffset;

	/** Projectile class to spawn */
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
		TSubclassOf<class ASurvivalGameProjectile> ProjectileClass;

	/** Sound to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		class USoundBase* FireSound;

	/** AnimMontage to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		class UAnimMontage* FireAnimation;

	/** Whether to use motion controller location for aiming. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		uint32 bUsingMotionControllers : 1;


	UFUNCTION(BlueprintCallable, Category = "Loadout")
		void SetupWithLoadout(int32 loadoutID);

	UFUNCTION(BlueprintCallable, Category = "Health")
		float GetCurrentHealth();

	UFUNCTION(BlueprintCallable, Category = "Health")
		UStat* GetHealthStat();

	UFUNCTION(BlueprintCallable, Category = "Text")
		static FText GetTextFromLiteral(FName text);

	UFUNCTION(BlueprintCallable, Category = "Health")
		void SetCurrentHealth(float val);

	UFUNCTION(BlueprintCallable, Category = "Health")
		float GetMaxHealth();

	UFUNCTION(BlueprintCallable, Category = "Health")
		void SetMaxHealth(float val);

	UFUNCTION(BlueprintCallable, Category = "Group")
		UGroup* GetGroup() { return group; }

	UFUNCTION(BlueprintCallable, Category = "Group")
		void SetGroup(UGroup* val) { group = val; }

	UFUNCTION(BlueprintCallable, Category = "Stats")
		TArray<UStat*>& GetStats() { return stats; }

	UFUNCTION(BlueprintCallable, Category = "Stats")
		UStat* GetStatByName(FText statName);

	UFUNCTION(BlueprintCallable, Category = "Name")
		FText GetCharacterName() { return characterName; }

	UFUNCTION(BlueprintCallable, Category = "Name")
		void SetCharacterName(FText val) { characterName = val; }

	UFUNCTION(BlueprintCallable, Category = "Weapon")
		TMap<EPosition, UWeapon*> GetWeapons() { return equipedWeapons; }

	UFUNCTION(BlueprintCallable, Category = "Weapon")
		void SetWeapons(TMap<EPosition, UWeapon*> val) { equipedWeapons = val; }

	UFUNCTION(BlueprintCallable, Category = "Armour")
		TMap<EPosition, UArmour*>& GetArmour() { return armour; }

	UFUNCTION(BlueprintCallable, Category = "Armour")
		void SetArmour(TMap<EPosition, UArmour*> val) { armour = val; }
	void ChangeHealth(float healthChange, bool heals);


protected:
	UInventory* inventory;
	USkillTree* skillTree;
	TArray<UAbility*> abilities;
	TMap<EPosition, UWeapon*> equippedWeapons;

	/** Fires a projectile. */
	void OnFire();

	/** Resets HMD orientation and position in VR. */
	void OnResetVR();

	/** Handles moving forward/backward */
	void MoveForward(float Val);

	/** Handles stafing movement, left and right */
	void MoveRight(float Val);

	/**
	 * Called via input to turn at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	struct TouchData
	{
		TouchData() { bIsPressed = false; Location = FVector::ZeroVector; }
		bool bIsPressed;
		ETouchIndex::Type FingerIndex;
		FVector Location;
		bool bMoved;
	};
	void BeginTouch(const ETouchIndex::Type FingerIndex, const FVector Location);
	void EndTouch(const ETouchIndex::Type FingerIndex, const FVector Location);
	void TouchUpdate(const ETouchIndex::Type FingerIndex, const FVector Location);
	TouchData	TouchItem;

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	// End of APawn interface

	/*
	 * Configures input for touchscreen devices if there is a valid touch interface for doing so
	 *
	 * @param	InputComponent	The input component pointer to bind controls to
	 * @returns true if touch controls were enabled.
	 */
	bool EnableTouchscreenMovement(UInputComponent* InputComponent);

public:
	/** Returns Mesh1P subobject **/
	FORCEINLINE class USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	/** Returns FirstPersonCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

};

