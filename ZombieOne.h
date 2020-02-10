// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyBaseClass.h"
#include "ZombieOne.generated.h"

/**
 * 
 */
UCLASS()
class SURVIVALGAME_API AZombieOne : public AEnemyBaseClass
{
	GENERATED_BODY()
	
public:
	
	virtual void Update() override;


};
