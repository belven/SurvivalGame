// Fill out your copyright notice in the Description page of Project Settings.

#include "ZombieOne.h"
#include "SurvivalGameCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "engine.h"
void AZombieOne::Update() 
{
	//EnemyBaseClass::Update();
	FVector location;
	ACharacter* player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

	FRotator weSmokeOpps;
	FVector whereHeAt;

	whereHeAt = player->GetActorLocation();
	
	weSmokeOpps = UKismetMathLibrary::FindLookAtRotation(this->GetActorLocation(), whereHeAt);

	this->SetActorRotation(weSmokeOpps);
	
	FVector whereImAt;
	whereImAt = this->GetActorLocation();
	whereImAt += this->GetActorForwardVector() * 2.0f;
	this->SetActorRelativeLocation(whereImAt);


}