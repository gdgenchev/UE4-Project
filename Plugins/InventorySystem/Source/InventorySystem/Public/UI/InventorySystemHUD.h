// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "GameFramework/PlayerController.h"
#include "UserWidgets/CollectedResourcesUserWidget.h"
#include "UserWidgets/InventoryUserWidget.h"
#include "Blueprint/UserWidget.h"

#include "InventorySystemHUD.generated.h"


UCLASS()
class INVENTORYSYSTEM_API AInventorySystemHUD : public AHUD
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	TSubclassOf<UUserWidget> CollectedResourcesUserWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	TSubclassOf<UUserWidget> InventoryUserWidgetClass;

	virtual void BeginPlay() override;
public:
	UCollectedResourcesUserWidget* GetCollectedResourcesUserWidget();

	UInventoryUserWidget* GetInventoryUserWidget();

	AInventorySystemHUD();
private:
	UPROPERTY()
	UCollectedResourcesUserWidget* CollectedResourcesUserWidget;

	UPROPERTY()
	UInventoryUserWidget* InventoryUserWidget;

	void InitializeCollectedResourcesUserWidget();
	void InitializeInventoryUserWidget();
};
