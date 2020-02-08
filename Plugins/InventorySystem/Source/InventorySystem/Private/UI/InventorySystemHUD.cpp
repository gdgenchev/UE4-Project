// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySystemHUD.h"
#include "InventorySystem.h"

AInventorySystemHUD::AInventorySystemHUD()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AInventorySystemHUD::BeginPlay()
{
	Super::BeginPlay();

	InitializeCollectedResourcesUserWidget();
	InitializeInventoryUserWidget();
}

void AInventorySystemHUD::InitializeCollectedResourcesUserWidget()
{
	if (IsValid(CollectedResourcesUserWidgetClass) == false)
	{
		UE_LOG(LogInventorySystem, Error, TEXT("%s: CollectedResourcesUserWidgetClass is not set from Blueprint"), *CLASS_FUNCTION);
		return;
	}

	CollectedResourcesUserWidget = CreateWidget<UCollectedResourcesUserWidget>(GetOwningPlayerController(), CollectedResourcesUserWidgetClass);

	if (IsValid(CollectedResourcesUserWidget))
	{
		CollectedResourcesUserWidget->AddToViewport();
	}
	else
	{
		UE_LOG(LogInventorySystem, Error, TEXT("%s: CollectedResourcesWidget is NULL"), *CLASS_FUNCTION);

	}
}

void AInventorySystemHUD::InitializeInventoryUserWidget()
{
	if (IsValid(InventoryUserWidgetClass) == false)
	{
		UE_LOG(LogInventorySystem, Error, TEXT("%s: InventoryUserWidgetClass is not set from Blueprint"), *CLASS_FUNCTION);
		return;
	}

	InventoryUserWidget = CreateWidget<UInventoryUserWidget>(GetOwningPlayerController(), InventoryUserWidgetClass);

	if (IsValid(InventoryUserWidget))
	{
		InventoryUserWidget->AddToViewport();
	}
	else
	{
		UE_LOG(LogInventorySystem, Error, TEXT("%s: InventoryUserWidget is NULL"), *CLASS_FUNCTION);
	}
}

UCollectedResourcesUserWidget* AInventorySystemHUD::GetCollectedResourcesUserWidget()
{
	return CollectedResourcesUserWidget;
}

UInventoryUserWidget* AInventorySystemHUD::GetInventoryUserWidget()
{
	return InventoryUserWidget;
}
