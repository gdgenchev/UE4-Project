// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Resource.h"
#include "Containers/Array.h"
#include "Containers/Map.h"
#include "UserWidgets/CollectedResourcesUserWidget.h"
#include "UserWidgets/InventoryUserWidget.h"
#include "InventoryResource.h"

#include "InventoryComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class INVENTORYSYSTEM_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UInventoryComponent();

	bool AddResources(TArray<UResource*> Resources);

	bool CanCarryResources(TArray<UResource*> Resources);
protected:
	virtual void BeginPlay() override;

	void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	TMap<FName, UInventoryResource*> InventoryResourceMap;

	UPROPERTY()
	UCollectedResourcesUserWidget* CollectedResourcesUserWidget;

	UPROPERTY()
	UInventoryUserWidget* InventoryUserWidget;

	bool bShouldCloseInventoryUI;

	void SavePointersToUserWidgetsFromHUD();

	void NotifyCollected(TMap<FName, int32> ResourceNameToCollectedAmount);

	void HandleInventoryUITrigger();

	void DisplayInventory();

	bool CanCarryResource(FName Resource);
};
