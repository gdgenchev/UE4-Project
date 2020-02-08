#pragma once

#include "Blueprint/UserWidget.h"

#include "InteractableUserWidget.generated.h"

UCLASS(BlueprintType)
class INVENTORYSYSTEM_API UInteractableUserWidget : public UUserWidget {
	GENERATED_BODY()

public:
	UInteractableUserWidget(const FObjectInitializer& ObjectInitializer);

	UImage* GetHintImage();

	UImage* GetTriggerImage();

	UTextBlock* GetText();
protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UImage* HintImage;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UImage* TriggerImage;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock* Text;
};