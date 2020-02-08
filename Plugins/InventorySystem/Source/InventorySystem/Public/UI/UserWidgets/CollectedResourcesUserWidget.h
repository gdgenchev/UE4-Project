// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Engine/Texture2D.h"
#include "Components/VerticalBox.h"

#include "CollectedResourcesUserWidget.generated.h"

UCLASS(BlueprintType)
class INVENTORYSYSTEM_API UCollectedResourcesUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UCollectedResourcesUserWidget(const FObjectInitializer& ObjectInitializer);

	void AddCollectedResource(int32 Amount, UTexture2D* Texture);

	void DisplayCollectedResources();

	void Clear();
protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UVerticalBox* CollectedResources;

	UPROPERTY(EditDefaultsOnly)
	int32 DisplayTime = 1;
private:
	FTimerManager* TimerManager = nullptr;
	FTimerHandle CollectedResourcesTimerHandle;
};
