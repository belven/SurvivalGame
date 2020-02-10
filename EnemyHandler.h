// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyBaseClass.h"
#include "EnemyHandler.generated.h"

UCLASS()
class SURVIVALGAME_API AEnemyHandler : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemyHandler();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	void UpdateEnemies();
	void CheckInRange();
	
	void SpawnEnemies();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	void DecreaseHealth(int enemyNumber, int healthDecrease);
	//This is a list of enemies that have been spawned
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemies")
		TArray<AEnemyBaseClass*> spawnedEnemyReferences;

	TArray<FVector> spawnedEnemyLocations;
	TArray<int> enemyTypeID;
	TArray<bool> withinRange;
	TArray<bool> alerted;
	TArray<float> health;
	TArray<int> enemyIDList;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemies")
		TSubclassOf<class AEnemyBaseClass> enemiesToInclude;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemies")
		int numberOfEnemies;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
		float min_X_Bound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
		float max_X_Bound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
		float min_Y_Bound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
		float max_Y_Bound;	

	

	FVector LocationOfPlayer;
	float timer;

};
