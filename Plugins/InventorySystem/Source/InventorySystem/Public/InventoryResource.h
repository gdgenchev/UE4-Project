// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/Texture2D.h"

#include "InventoryResource.generated.h"

UCLASS()
class INVENTORYSYSTEM_API UInventoryResource : public UObject
{
	GENERATED_BODY()
private:
    FName Name;
    UTexture2D* Texture;
    int32 CurrentAmount;
    int32 MaxAmount;

public:
    UInventoryResource();

    void Initialize(FName Name, UTexture2D* Texture, int32 CurrentAmount, int32 MaxAmount);

    int32 AddToCurrentAmount(int32 Amount);

    bool IsFull();

    UTexture2D* GetTexture();

    int32 GetCurrentAmount();

    int32 GetMaxAmount();
};
