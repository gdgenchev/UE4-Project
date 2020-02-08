#include "InventoryResource.h"

UInventoryResource::UInventoryResource()
{
	this->Name = FName(TEXT("undefined"));
	this->Texture = nullptr;
	this->CurrentAmount = 0;
	this->MaxAmount = 0;
}

void UInventoryResource::Initialize(FName Name, UTexture2D* Texture, int32 CurrentAmount, int32 MaxAmount)
{
	this->Name = Name;
	this->Texture = Texture;
	this->CurrentAmount = CurrentAmount;
	this->MaxAmount = MaxAmount;
}

int32 UInventoryResource::AddToCurrentAmount(int32 Amount)
{
	int32 LastCurrentAmount = CurrentAmount;
	if (CurrentAmount + Amount <= MaxAmount)
	{
		CurrentAmount += Amount;
	}
	else
	{
		CurrentAmount = MaxAmount;
	}

	return CurrentAmount - LastCurrentAmount;
}

bool UInventoryResource::IsFull()
{
	return CurrentAmount == MaxAmount;
}

UTexture2D* UInventoryResource::GetTexture()
{
	return Texture;
}

int32 UInventoryResource::GetCurrentAmount()
{
	return CurrentAmount;
}

int32 UInventoryResource::GetMaxAmount()
{
	return MaxAmount;
}
