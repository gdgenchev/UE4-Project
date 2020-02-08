// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/WrapBox.h"
#include "Engine/Texture2D.h"
#include "InventoryResource.h"

#include "InventoryUserWidget.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class INVENTORYSYSTEM_API UInventoryUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UInventoryUserWidget(const FObjectInitializer& ObjectInitializer);

	void AddResource(UInventoryResource* Resource);

	void DisplayInventory();

	void ClearInventory();
protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UWrapBox* InventoryWrapBox;
	
};
