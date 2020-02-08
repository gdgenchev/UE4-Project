// Fill out your copyright notice in the Description page of Project Settings.


#include "CollectedResourcesUserWidget.h"
#include "Components/SlateWrapperTypes.h"
#include "WidgetBlueprintLibrary.h"
#include "Components/HorizontalBox.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/HorizontalBoxSlot.h"
#include "InventorySystem.h"


UCollectedResourcesUserWidget::UCollectedResourcesUserWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	CollectedResources = NewObject<UVerticalBox>(UVerticalBox::StaticClass());
	SetVisibility(ESlateVisibility::Hidden);

}

void UCollectedResourcesUserWidget::AddCollectedResource(int32 Amount, UTexture2D* Texture)
{
	UImage* Image = NewObject<UImage>(UImage::StaticClass());
	Image->SetBrush(UWidgetBlueprintLibrary::MakeBrushFromTexture(Texture, 40, 40));

	UTextBlock* CollectedAmountText = NewObject<UTextBlock>(UTextBlock::StaticClass());
	CollectedAmountText->Font.Size = 16;
	if (Amount > 0)
	{
		CollectedAmountText->SetText(FText::FromString("+" + FString::FromInt(Amount)));
	}
	else
	{
		CollectedAmountText->SetText(FText::FromString("FULL"));
		CollectedAmountText->SetColorAndOpacity(FSlateColor(FColor::Green));
	}

	UHorizontalBox* HorizontalBox = NewObject<UHorizontalBox>(UHorizontalBox::StaticClass());
	UHorizontalBoxSlot* LeftSlot = HorizontalBox->AddChildToHorizontalBox(CollectedAmountText);
	UHorizontalBoxSlot* RightSlot = HorizontalBox->AddChildToHorizontalBox(Image);
	LeftSlot->SetSize(ESlateSizeRule::Fill);
	LeftSlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Right);
	RightSlot->SetSize(ESlateSizeRule::Fill);
	RightSlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Left);

	CollectedResources->AddChild(HorizontalBox);
}

void UCollectedResourcesUserWidget::DisplayCollectedResources()
{
	SetVisibility(ESlateVisibility::Visible);
	UWorld* World = GetWorld();
	if (IsValid(World) == false)
	{
		UE_LOG(LogInventorySystem, Error, TEXT("%s: World is nullptr"), *CLASS_FUNCTION);
		return;
	}

	TimerManager = &World->GetTimerManager();
	TimerManager->SetTimer(CollectedResourcesTimerHandle, this, &UCollectedResourcesUserWidget::Clear, DisplayTime);
}

void UCollectedResourcesUserWidget::Clear()
{
	CollectedResources->ClearChildren();
	SetVisibility(ESlateVisibility::Hidden);

}
