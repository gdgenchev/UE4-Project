#include "InteractableUserWidget.h"


UInteractableUserWidget::UInteractableUserWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}

UImage* UInteractableUserWidget::GetHintImage()
{
	return HintImage;
}

UImage* UInteractableUserWidget::GetTriggerImage()
{
	return TriggerImage;
}

UTextBlock* UInteractableUserWidget::GetText()
{
	return Text;
}
