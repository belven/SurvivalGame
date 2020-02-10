// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemyBaseClass.h"
#include "EnemyHandler.h"

// Sets default values
AEnemyBaseClass::AEnemyBaseClass()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AEnemyBaseClass::BeginPlay()
{
	Super::BeginPlay();
	
}

void AEnemyBaseClass::OnSpawn(AEnemyHandler* theOne)
{
	//theHandler = theOne;
	AEnemyHandler* kit = theOne;
	theHandler = kit;
}

// Called every frame
void AEnemyBaseClass::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemyBaseClass::SetIterationID(int idToSetItTo)
{
	EnemyIterationID = idToSetItTo;
}

void AEnemyBaseClass::Update()
{

}

void AEnemyBaseClass::SetGlobalID(int ID)
{
	globalIdentifier = ID;
}

void AEnemyBaseClass::DamageTarget(float Damage)
{
	theHandler->DecreaseHealth(globalIdentifier, Damage);
}
