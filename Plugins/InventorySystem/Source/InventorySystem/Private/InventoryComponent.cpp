// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Datatables/InventoryResourceStruct.h"
#include "Engine/DataTable.h"
#include "UI/InventorySystemHUD.h"
#include "InventorySystem.h"
#include "GameFramework/PlayerInput.h"


// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UDataTable> InventoryItemsDataObject(
		TEXT("DataTable'/InventorySystem/Blueprints/DataTables/BP_InventoryResourcesDatatable.BP_InventoryResourcesDatatable'"));
	if (InventoryItemsDataObject.Succeeded() == false)
	{
		return;
	}

	UDataTable* InventoryDataTable = InventoryItemsDataObject.Object;

	TArray<FInventoryResourceStruct*> Rows;
	const FString ContextString(TEXT("Get all rows from inventory datatable"));
	InventoryDataTable->GetAllRows(ContextString, Rows);

	for (int i = 0; i < Rows.Num(); i++)
	{
		UInventoryResource* InventoryResource = NewObject<UInventoryResource>(UInventoryResource::StaticClass());
		InventoryResource->Initialize(Rows[i]->Name, Rows[i]->Texture, 0, Rows[i]->MaxAmount);
		InventoryResourceMap.Add(Rows[i]->Name, InventoryResource);
	}
}

// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
	SavePointersToUserWidgetsFromHUD();
}

void UInventoryComponent::SavePointersToUserWidgetsFromHUD()
{
	UWorld* World = GetWorld();
	if (IsValid(World) == false)
	{
		UE_LOG(LogInventorySystem, Error, TEXT("%s: GetWorld() returned NULL"), *CLASS_FUNCTION);
		return;
	}

	APlayerController* Controller = Cast<APlayerController>(World->GetFirstPlayerController());
	if (IsValid(Controller) == false)
	{
		UE_LOG(LogInventorySystem, Error, TEXT("%s:GetFirstPlayerController() returned NULL"), *CLASS_FUNCTION);
		return;
	}

	AInventorySystemHUD* HUD = Cast<AInventorySystemHUD>(Controller->GetHUD());
	if (IsValid(HUD))
	{
		this->CollectedResourcesUserWidget = HUD->GetCollectedResourcesUserWidget();
		this->InventoryUserWidget = HUD->GetInventoryUserWidget();
	}
	else
	{
		UE_LOG(LogInventorySystem, Error, TEXT("%s: Controller->GetHud() returned NULL"), *CLASS_FUNCTION);
	}
}

void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	HandleInventoryUITrigger();
}

void UInventoryComponent::HandleInventoryUITrigger()
{
	UWorld* World = GetWorld();
	if (IsValid(World) == false)
	{
		UE_LOG(LogInventorySystem, Error, TEXT("%s: GetWorld() returned NULL"), *CLASS_FUNCTION);
		return;
	}

	APlayerController* MainCharacterController = World->GetFirstPlayerController();

	if (IsValid(MainCharacterController) == false)
	{
		UE_LOG(LogInventorySystem, Error, TEXT("%s: World->GetFirstPlayerController() returned NULL"), *CLASS_FUNCTION);
		return;
	}

	if (MainCharacterController->WasInputKeyJustPressed(EKeys::I))
	{
		if (bShouldCloseInventoryUI == true)
		{
			MainCharacterController->SetPause(false);
			bShouldCloseInventoryUI = false;
			InventoryUserWidget->ClearInventory();
			return;
		}
	}

	if (MainCharacterController->WasInputKeyJustPressed(EKeys::I))
	{
		if (bShouldCloseInventoryUI == false)
		{
			SetTickableWhenPaused(true);
			MainCharacterController->Pause();
			bShouldCloseInventoryUI = true;
			UE_LOG(LogInventorySystem, Display, TEXT("I was pressed daiba"));
			DisplayInventory();
		}
	}
}

void UInventoryComponent::DisplayInventory()
{
	for (auto& Elem : InventoryResourceMap)
	{
		UInventoryResource* Resource = Elem.Value;
		InventoryUserWidget->AddResource(Resource);
	}

	InventoryUserWidget->DisplayInventory();
}

bool UInventoryComponent::AddResources(TArray<UResource*> Resources)
{
	bool bCollectedSomething = false;

	TMap<FName, int32> ResourceNameToCollectedAmount;

	for (auto& Resource : Resources)
	{
		FName ResourceName = Resource->GetName();
		int32 AmountToAdd = Resource->GetAmount();

		int32 ActualAddedAmount = InventoryResourceMap[ResourceName]->AddToCurrentAmount(AmountToAdd);

		if (ActualAddedAmount > 0)
		{
			bCollectedSomething = true;
		}

		ResourceNameToCollectedAmount.Add(ResourceName, ActualAddedAmount);
	}

	if (bCollectedSomething)
	{
		NotifyCollected(ResourceNameToCollectedAmount);
	}

	return bCollectedSomething;
}

void UInventoryComponent::NotifyCollected(TMap<FName, int32> ResourceNameToCollectedAmount)
{
	if (IsValid(CollectedResourcesUserWidget) == false)
	{
		UE_LOG(LogInventorySystem, Error, TEXT("%s: CollectedResourcesWidget is nullptr"), *CLASS_FUNCTION);
		return;
	}

	for (auto& Elem : ResourceNameToCollectedAmount)
	{
		FName ResourceName = Elem.Key;
		int32 CollectedAmount = Elem.Value;
		CollectedResourcesUserWidget->AddCollectedResource(CollectedAmount, InventoryResourceMap[ResourceName]->GetTexture());
	}

	CollectedResourcesUserWidget->DisplayCollectedResources();
}


bool UInventoryComponent::CanCarryResources(TArray<UResource*> Resources)
{
	bool bCanCarryResources = false;

	for (auto& Resource : Resources)
	{
		bCanCarryResources |= CanCarryResource(Resource->GetName());
	}

	return bCanCarryResources;
}

bool UInventoryComponent::CanCarryResource(FName ResourceName)
{
	UInventoryResource* InventoryResource = InventoryResourceMap[ResourceName];

	return !InventoryResource->IsFull();
}