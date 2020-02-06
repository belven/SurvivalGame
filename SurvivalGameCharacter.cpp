// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "SurvivalGameCharacter.h"
#include "SurvivalGameProjectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/InputSettings.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "MotionControllerComponent.h"
#include "XRMotionControllerBase.h" // for FXRMotionControllerBase::RightHandSourceId
#include "Stat.h"
#include "Items/Weapon.h"
#include "Items/Armour/Armour.h"
#include "Datatables/DataTables.h"
#include "Items/ItemContainer.h"
#include "Engine.h"

DEFINE_LOG_CATEGORY_STATIC(LogFPChar, Warning, All);

const FText ASurvivalGameCharacter::healthStatName = GetTextFromLiteral(TEXT("Health"));

//////////////////////////////////////////////////////////////////////////
// ASurvivalGameCharacter

ASurvivalGameCharacter::ASurvivalGameCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-39.56f, 1.75f, 64.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->SetRelativeRotation(FRotator(1.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-0.5f, -4.4f, -155.7f));

	// Create a gun mesh component
	FP_Gun = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FP_Gun"));
	FP_Gun->SetOnlyOwnerSee(true);			// only the owning player will see this mesh
	FP_Gun->bCastDynamicShadow = false;
	FP_Gun->CastShadow = false;
	// FP_Gun->SetupAttachment(Mesh1P, TEXT("GripPoint"));
	FP_Gun->SetupAttachment(RootComponent);

	FP_MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocation"));
	FP_MuzzleLocation->SetupAttachment(FP_Gun);
	FP_MuzzleLocation->SetRelativeLocation(FVector(0.2f, 48.4f, -10.6f));

	// Default offset from the character location for projectiles to spawn
	GunOffset = FVector(100.0f, 0.0f, 10.0f);

	// Note: The ProjectileClass and the skeletal mesh/anim blueprints for Mesh1P, FP_Gun, and VR_Gun 
	// are set in the derived blueprint asset named MyCharacter to avoid direct content references in C++.

	// Create VR Controllers.
	R_MotionController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("R_MotionController"));
	R_MotionController->MotionSource = FXRMotionControllerBase::RightHandSourceId;
	R_MotionController->SetupAttachment(RootComponent);
	L_MotionController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("L_MotionController"));
	L_MotionController->SetupAttachment(RootComponent);

	// Create a gun and attach it to the right-hand VR controller.
	// Create a gun mesh component
	VR_Gun = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("VR_Gun"));
	VR_Gun->SetOnlyOwnerSee(true);			// only the owning player will see this mesh
	VR_Gun->bCastDynamicShadow = false;
	VR_Gun->CastShadow = false;
	VR_Gun->SetupAttachment(R_MotionController);
	VR_Gun->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));

	VR_MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("VR_MuzzleLocation"));
	VR_MuzzleLocation->SetupAttachment(VR_Gun);
	VR_MuzzleLocation->SetRelativeLocation(FVector(0.000004, 53.999992, 10.000000));
	VR_MuzzleLocation->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));		// Counteract the rotation of the VR gun model.

	// Uncomment the following line to turn motion controllers on by default:
	//bUsingMotionControllers = true;
}

void ASurvivalGameCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Attach gun mesh component to Skeleton, doing it here because the skeleton is not yet created in the constructor
	FP_Gun->AttachToComponent(Mesh1P, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint"));

	// Show or hide the two versions of the gun based on whether or not we're using motion controllers.
	if (bUsingMotionControllers)
	{
		VR_Gun->SetHiddenInGame(false, true);
		Mesh1P->SetHiddenInGame(true, true);
	}
	else
	{
		VR_Gun->SetHiddenInGame(true, true);
		Mesh1P->SetHiddenInGame(false, true);
	}
}

//////////////////////////////////////////////////////////////////////////
// Input


void ASurvivalGameCharacter::SetupWithLoadout(int32 loadoutID) {
	FLoadout* ourloadout = nullptr;

	for (FLoadout* loadout : UDataTables::GetInstance()->GetLoadouts()) {
		if (loadout->characterID == this->ID) {
			ourloadout = loadout;
			break;
		}
	}

	if (ourloadout != nullptr) {
		SetMaxHealth(ourloadout->maxHealth);

		for (TPair<EPosition, int32>& weaponPosition : ourloadout->equippedWeapons) {
			TPair<EPosition, UWeapon*> weaponPair;
			weaponPair.Key = weaponPosition.Key;
			weaponPair.Value = Cast<UWeapon>(UItemContainer::LoadItem(weaponPosition.Value));

			GetWeapons().Add(weaponPair);
		}

		/*for (int32 armourID : ourloadout->equippedArmour) {
			UArmour* armourFound = Cast<UArmour>(UItemContainer::LoadItem(armourID));

			if (armourFound != nullptr) {
				TPair<EPosition, UArmour*> armourPair;
				armourPair.Key = armourFound->GetArmourSpecification()->armourPosition;
				armourPair.Value = armourFound;

				GetArmour().Add(armourPair);
			}
		}*/

		MaximiseStats();
	}
}

void ASurvivalGameCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// set up gameplay key bindings
	check(PlayerInputComponent);

	// Bind jump events
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	// Bind fire event
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ASurvivalGameCharacter::OnFire);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &ASurvivalGameCharacter::StopFire);

	//Bind aim down sights event 
	PlayerInputComponent->BindAction("Aim", IE_Pressed, this, &ASurvivalGameCharacter::OnAim);
	PlayerInputComponent->BindAction("Aim", IE_Released, this, &ASurvivalGameCharacter::OnHip);

	// Enable touchscreen input
	EnableTouchscreenMovement(PlayerInputComponent);

	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &ASurvivalGameCharacter::OnResetVR);

	// Bind movement events
	PlayerInputComponent->BindAxis("MoveForward", this, &ASurvivalGameCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASurvivalGameCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &ASurvivalGameCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &ASurvivalGameCharacter::LookUpAtRate);
}

//******************************************************These are all weapon related functions***********************************************************************//

//This requires, for prototyping purposes, the individual building the game to set the array values during runtime to find correct weapon locations. This allows 
//the prototyper freedom to experiemnt with as many weapons as they wish. blueprint callable allows weapons to be equipped with minimal effort by the prototyper

//this is used to test for automatic firing status
void ASurvivalGameCharacter::CallFullAuto()
{
	if (isFiring == true) 
	{
		FullyAutomaticFire();
	}
}

//This is callable by blueprints; Sets the mesh and transforms of the player's held weapon upon switching weapons, as well as how much damage should be dealt.
void ASurvivalGameCharacter::changeGunEquipped(int gunNumber, int weaponDamage, int weapontype, int currentRateOfFire)
{
	FP_Gun->SetSkeletalMesh(gunList[gunNumber]);
	FP_Gun->SetRelativeRotation(relativeGunRotations[gunNumber]);
	FP_Gun->SetRelativeLocation(relativeGunLocation[gunNumber]);
	FP_Gun->SetWorldScale3D(gunScale[gunNumber]);	
	FP_MuzzleLocation->SetRelativeLocation(relativeMuzzleLocation[gunNumber]);
	//DamageToDealToEnemy = weaponDamage
	currentFireType = weapontype;
	rateOfFire = currentRateOfFire;
}

//called when left mouse button is released
void ASurvivalGameCharacter::StopFire()
{
	isFiring = false;
}

//called at left mouse button click for firing
void ASurvivalGameCharacter::OnFire()
{
	if (currentFireType == 0) //semi-automatic
	{
		SemiAutomaticFire();
	}
	else if (currentFireType == 1) //fully automatic
	{
		//fire a shot immediately
		firearmTimer = 0;

		//lets the function that checks this bool know that the trigger is being pressed on an automatic weapon
		isFiring = true;
	}

}

//These 2 functions handle aiming
void ASurvivalGameCharacter::OnAim()
{
	FirstPersonCameraComponent->SetFieldOfView(70.0f);
	FP_Gun->SetVisibility(false);
	//FP_GunADS->SetVisibility(true);
}

void ASurvivalGameCharacter::OnHip()
{
	FirstPersonCameraComponent->SetFieldOfView(90.0f);
	FP_Gun->SetVisibility(true);
	//FP_GunADS->SetVisibility(false);
}

//this creates a raycast upon firing for dealing damage, calls other related functions; this is called by semiautomaticfire and fullyautomaticfire
void ASurvivalGameCharacter::DoRayCast() 
{
	FHitResult* HitResult = new FHitResult();
	FVector StartTrace = FirstPersonCameraComponent->GetComponentLocation();
	FVector forwardVector = FirstPersonCameraComponent->GetForwardVector();
	FVector EndTrace = ((forwardVector*20000.f) + StartTrace);
	FCollisionQueryParams* TraceParams = new FCollisionQueryParams();



	//this is a raycast from the player character camera
	if (GetWorld()->LineTraceSingleByChannel(*HitResult, StartTrace, EndTrace, ECC_Visibility, *TraceParams))
	{
		//makes line
		DrawDebugLine(GetWorld(), StartTrace, EndTrace, FColor(255, 0, 0), true);
		//makes message
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("You hit: %s"), *HitResult->Actor->GetName()));

		//ASurvivalGameEnemyBase* TestTarget = Cast<AMyEnemyBase>(HitResult->Actor.Get());

		//if (TestTarget != NULL && !TestTarget->IsPendingKill())
		//{
		//	TestTarget->DamageTarget(gunBaseDamage);
		//}

		FVector_NetQuantizeNormal Locs;
		FRotator Rots;
		Locs = HitResult->Location;
		//makes bullet impact, animation, sound, and muzzle flash + gun smoke
		SpawnBulletImpact(Locs, Rots);
		FireSoundAndAnimation();
		SpawnGunSmoke();
		
	}
}

//called by onfire
void ASurvivalGameCharacter::SemiAutomaticFire() 
{
	DoRayCast();
	//ammoInMagazine -=1;
}
//called by a timer or tick when isFiring = true
void ASurvivalGameCharacter::FullyAutomaticFire() 
{
	firearmTimer -= 1;
	if (firearmTimer < 5)
	{
		DoRayCast();
		//ammoInMagazine -=1;

		//reset timer, buffer between shots. rateOfFire is set on weapon switch 
		firearmTimer = rateOfFire;
	}
	
}

//this is called by doraycast
void ASurvivalGameCharacter::FireSoundAndAnimation() 
{
	// try and play the sound if specified
	if (FireSound != NULL)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
	}

	// try and play a firing animation if specified
	if (FireAnimation != NULL)
	{
		// Get the animation object for the arms mesh
		UAnimInstance* AnimInstance = Mesh1P->GetAnimInstance();
		if (AnimInstance != NULL)
		{
			AnimInstance->Montage_Play(FireAnimation, 1.f);
		}
	}
}

//this is called by doraycast
void ASurvivalGameCharacter::SpawnGunSmoke()
{
	FRotator SpawnRotation;
	SpawnRotation = GetControlRotation();

	FVector x;
	x = ((FP_MuzzleLocation != nullptr) ? FP_MuzzleLocation->GetComponentLocation() : GetActorLocation()) + SpawnRotation.RotateVector(GunOffset);


	FActorSpawnParameters SpawnParams;
	AActor* SPawnedActorRef = GetWorld()->SpawnActor<AActor>(GunSmoke, x, SpawnRotation, SpawnParams);
}

//this is called by doraycast
void ASurvivalGameCharacter::SpawnBulletImpact(FVector Loc, FRotator Rot)
{
	FActorSpawnParameters SpawnParams;
	AActor* SpawnedActorRef = GetWorld()->SpawnActor<AActor>(BulletImpact, Loc, Rot, SpawnParams);
}

//****************************************************************END Weapon Mechanics*******************************************************************************//

void ASurvivalGameCharacter::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void ASurvivalGameCharacter::BeginTouch(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	if (TouchItem.bIsPressed == true)
	{
		return;
	}
	if ((FingerIndex == TouchItem.FingerIndex) && (TouchItem.bMoved == false))
	{
		OnFire();
	}
	TouchItem.bIsPressed = true;
	TouchItem.FingerIndex = FingerIndex;
	TouchItem.Location = Location;
	TouchItem.bMoved = false;
}

void ASurvivalGameCharacter::EndTouch(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	if (TouchItem.bIsPressed == false)
	{
		return;
	}
	TouchItem.bIsPressed = false;
}

//Commenting this section out to be consistent with FPS BP template.
//This allows the user to turn without using the right virtual joystick

//void ASurvivalGameCharacter::TouchUpdate(const ETouchIndex::Type FingerIndex, const FVector Location)
//{
//	if ((TouchItem.bIsPressed == true) && (TouchItem.FingerIndex == FingerIndex))
//	{
//		if (TouchItem.bIsPressed)
//		{
//			if (GetWorld() != nullptr)
//			{
//				UGameViewportClient* ViewportClient = GetWorld()->GetGameViewport();
//				if (ViewportClient != nullptr)
//				{
//					FVector MoveDelta = Location - TouchItem.Location;
//					FVector2D ScreenSize;
//					ViewportClient->GetViewportSize(ScreenSize);
//					FVector2D ScaledDelta = FVector2D(MoveDelta.X, MoveDelta.Y) / ScreenSize;
//					if (FMath::Abs(ScaledDelta.X) >= 4.0 / ScreenSize.X)
//					{
//						TouchItem.bMoved = true;
//						float Value = ScaledDelta.X * BaseTurnRate;
//						AddControllerYawInput(Value);
//					}
//					if (FMath::Abs(ScaledDelta.Y) >= 4.0 / ScreenSize.Y)
//					{
//						TouchItem.bMoved = true;
//						float Value = ScaledDelta.Y * BaseTurnRate;
//						AddControllerPitchInput(Value);
//					}
//					TouchItem.Location = Location;
//				}
//				TouchItem.Location = Location;
//			}
//		}
//	}
//}

void ASurvivalGameCharacter::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void ASurvivalGameCharacter::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void ASurvivalGameCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ASurvivalGameCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

bool ASurvivalGameCharacter::EnableTouchscreenMovement(class UInputComponent* PlayerInputComponent)
{
	if (FPlatformMisc::SupportsTouchInput() || GetDefault<UInputSettings>()->bUseMouseForTouch)
	{
		PlayerInputComponent->BindTouch(EInputEvent::IE_Pressed, this, &ASurvivalGameCharacter::BeginTouch);
		PlayerInputComponent->BindTouch(EInputEvent::IE_Released, this, &ASurvivalGameCharacter::EndTouch);

		//Commenting this out to be more consistent with FPS BP template.
		//PlayerInputComponent->BindTouch(EInputEvent::IE_Repeat, this, &ASurvivalGameCharacter::TouchUpdate);
		return true;
	}

	return false;
}

UStat* ASurvivalGameCharacter::GetStatByName(FText statName)
{
	for (UStat* stat : stats) {
		if (stat->GetStatName().EqualTo(statName))
			return stat;
	}
	return NewObject<UStat>();
}

void ASurvivalGameCharacter::ChangeHealth(float healthChange, bool heals)
{
	// Need to add in armour damage reduction
	float healthChangeAmout;

	if (!heals) {
		healthChangeAmout = -healthChange;
	}
	else {
		healthChangeAmout = healthChange;
	}

	SetCurrentHealth(GetCurrentHealth() - healthChangeAmout);
}

float ASurvivalGameCharacter::GetCurrentHealth()
{
	return GetHealthStat()->GetCurrentValue();
}

UStat* ASurvivalGameCharacter::GetHealthStat()
{
	return GetStatByName(ASurvivalGameCharacter::healthStatName);
}

FText ASurvivalGameCharacter::GetTextFromLiteral(FName text)
{
	return FText::FromName(text);
}

void ASurvivalGameCharacter::SetCurrentHealth(float val)
{
	GetHealthStat()->SetCurrentValue(val);
}

float ASurvivalGameCharacter::GetMaxHealth()
{
	return GetHealthStat()->GetMaxValue();
}

void ASurvivalGameCharacter::SetMaxHealth(float val)
{
	GetHealthStat()->SetMaxValue(val);
}

void ASurvivalGameCharacter::MaximiseStats()
{
	SetCurrentHealth(GetMaxHealth());
}

void ASurvivalGameCharacter::AddStat(UStat* newStat)
{
	if (GetStatByName(newStat->GetStatName()) == NULL)
		GetStats().Add(newStat);
}

bool ASurvivalGameCharacter::IsAlive()
{
	return GetCurrentHealth() > 0;
}

void ASurvivalGameCharacter::InteractWithTarget(ASurvivalGameCharacter* target)
{
	// Assumed Enemy for now. Need to implement friend foe system
	if (CanAttack() && IsAlive() && target->IsAlive()) {
		TArray<UWeapon*> weapons;

		equipedWeapons.GenerateValueArray(weapons);

		for (UWeapon* weapon : weapons) {
			weapon->AttackTarget(target);
		}
	}
}

bool ASurvivalGameCharacter::CanAttack()
{
	return equipedWeapons.Num() > 0;
}

