// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemyHandler.h"
#include "EnemyBaseClass.h"
#include "Engine.h"
#include "Math/UnrealMathUtility.h"

// Sets default values
AEnemyHandler::AEnemyHandler()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	numberOfEnemies = 5;
	health.Init(100.0f, numberOfEnemies);
	spawnedEnemyReferences.Init(nullptr, numberOfEnemies);
	enemyIDList.Init(0, numberOfEnemies);
	timer = 60.0f;
}

// Called when the game starts or when spawned
void AEnemyHandler::BeginPlay()
{
	Super::BeginPlay();
	SpawnEnemies();
	
}

void AEnemyHandler::SpawnEnemies()
{
	int x = 0;
	for (int i = 0; i < numberOfEnemies; i++)
	{
		FActorSpawnParameters SpawnParams;
		FVector spawnpoint;
		FRotator spawnRot;
		spawnpoint.Z = 300.0f;
		spawnpoint.X = FMath::FRandRange(min_X_Bound, max_X_Bound);
		spawnpoint.Y = FMath::FRandRange(min_Y_Bound, max_Y_Bound);
		spawnRot.Yaw   = 0.0f;
		spawnRot.Pitch = 0.0f;
		spawnRot.Roll  = 0.0f;		
		AEnemyBaseClass* newEnemy  = GetWorld()->SpawnActor<AEnemyBaseClass>(enemiesToInclude, spawnpoint, spawnRot, SpawnParams);
		health[i] = 100.0f;		
		spawnedEnemyReferences[i] = newEnemy;
		spawnedEnemyReferences[i]->OnSpawn(this);
		
		int enemyID = 715 + i;
		spawnedEnemyReferences[i]->SetGlobalID(enemyID);
		enemyIDList[i] = enemyID;

	}
}

// Called every frame
void AEnemyHandler::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	timer -= 15.0f;
	if (timer < 5.0f) 
	{
		UpdateEnemies();
		timer = 60.0f;
	}
}

void AEnemyHandler::UpdateEnemies()
{

	for (int i = 0; i < numberOfEnemies; i++) 
	{
		AEnemyBaseClass* EnemyToUpdate = Cast<AEnemyBaseClass>(spawnedEnemyReferences[i]);
		EnemyToUpdate->Update();
	}
}

void AEnemyHandler::DecreaseHealth(int enemyNumber, int healthDecrease)
{
	//enemy number is equivalent to the globalID
	int numberToUse = enemyNumber - 715;

	health[numberToUse] -= healthDecrease;

	if(health[numberToUse] <= 0)
	{
		spawnedEnemyReferences[numberToUse]->Destroy();
	}
}

void AEnemyHandler::CheckInRange()
{
}

