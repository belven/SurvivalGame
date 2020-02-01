// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Datatables/DataTables.h"
#include "UObject/Object.h"
#include "Item.generated.h"

/**
 * 
 */
UCLASS()
class SURVIVALGAME_API UItem : public UObject
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "Item Specification")
		FItemSpecification itemSpecification;

public:
	UFUNCTION(BlueprintCallable, Category = "Item Specification")
		FItemSpecification GetItemSpecification() { return itemSpecification; }

	UFUNCTION(BlueprintCallable, Category = "Item Specification")
		void SetItemSpecification(FItemSpecification val) { itemSpecification = val; }

	UFUNCTION(BlueprintCallable, Category = "Item")
		static UItem* CreateItem(int32 itemID, FItemSpecification inItemSpecification);
};
