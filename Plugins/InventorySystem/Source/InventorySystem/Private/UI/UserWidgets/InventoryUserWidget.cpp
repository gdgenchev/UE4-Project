// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryUserWidget.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"
#include "InventorySystem.h"
#include "WidgetBlueprintLibrary.h"

UInventoryUserWidget::UInventoryUserWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	InventoryWrapBox = NewObject<UWrapBox>(UWrapBox::StaticClass());
	SetVisibility(ESlateVisibility::Hidden);
}

void UInventoryUserWidget::AddResource(UInventoryResource* Resource)
{
	if (IsValid(InventoryWrapBox) == false)
	{
		UE_LOG(LogInventorySystem, Error, TEXT("%s: InventoryWrapBox is NULL"), *CLASS_FUNCTION);
		return;
	}

	UTexture2D* Texture = Resource->GetTexture();
	int32 CurrentAmount = Resource->GetCurrentAmount();
	int32 MaxAmount = Resource->GetMaxAmount();

	UImage* Image = NewObject<UImage>(UImage::StaticClass());
	Image->SetBrush(UWidgetBlueprintLibrary::MakeBrushFromTexture(Texture, 40, 40));

	UTextBlock* CurrentAmountText = NewObject<UTextBlock>(UTextBlock::StaticClass());
	//expose to blueprint property
	CurrentAmountText->Font.Size = 16;
	CurrentAmountText->Text = FText::FromString(FString::FromInt(CurrentAmount));

	if (CurrentAmount == 0)
	{
		CurrentAmountText->SetColorAndOpacity(FSlateColor(FColor::Red));
	}
	else if (CurrentAmount == MaxAmount)
	{
		CurrentAmountText->SetColorAndOpacity(FSlateColor(FColor::Green));
	}

	UVerticalBox* VerticalBox = NewObject<UVerticalBox>(UVerticalBox::StaticClass());
	UVerticalBoxSlot* TopSlot = VerticalBox->AddChildToVerticalBox(Image);
	UVerticalBoxSlot* BottomSlot = VerticalBox->AddChildToVerticalBox(CurrentAmountText);

	TopSlot->Padding.Right = 4;

	BottomSlot->SetSize(ESlateSizeRule::Fill);
	BottomSlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Center);
	BottomSlot->Padding.Right = 4;

	InventoryWrapBox->AddChild(VerticalBox);
}

void UInventoryUserWidget::DisplayInventory()
{
	SetVisibility(ESlateVisibility::Visible);
}

/* This method can be optimzed to HideInventory - 
 * we should just update the inventory when we add the resources
 * Same implies for the CollectedResourcesWidget
 */
void UInventoryUserWidget::ClearInventory()
{
	InventoryWrapBox->ClearChildren();
	SetVisibility(ESlateVisibility::Hidden);
}
