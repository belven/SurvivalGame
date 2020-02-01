// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Group.generated.h"

class ASurvivalGameCharacter;

UCLASS()
class SURVIVALGAME_API UGroup : public UObject
{
	GENERATED_BODY()
private:
	TArray<ASurvivalGameCharacter*> members;

public:
	UFUNCTION(BlueprintCallable, Category = "Group")
		TArray<ASurvivalGameCharacter *>& GetMembers() { return members; }

	UFUNCTION(BlueprintCallable, Category = "Group")
		void AddMember(ASurvivalGameCharacter* memberToAdd);

	UFUNCTION(BlueprintCallable, Category = "Group")
		void RemoveMember(ASurvivalGameCharacter* memberToRemove);

};
