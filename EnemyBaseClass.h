// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyBaseClass.generated.h"

UCLASS()
class SURVIVALGAME_API AEnemyBaseClass : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemyBaseClass();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	virtual void Update();

	UFUNCTION(BlueprintCallable, Category = "Management")
		void OnSpawn(AEnemyHandler* theOne);

	void DamageTarget(float damage);

	int EnemyIterationID;

	void SetIterationID(int idToSetItTo);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Handler")
		AEnemyHandler* theHandler;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BaseStats")
		int globalIdentifier;

	UFUNCTION(BlueprintCallable, Category = "BaseStats")
		void SetGlobalID(int ID);


};
