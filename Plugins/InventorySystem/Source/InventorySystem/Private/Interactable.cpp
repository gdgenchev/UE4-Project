// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactable.h"
#include "InventoryComponent.h"
#include "InventorySystem.h"
#include "InteractableUserWidget.h"
#include "WidgetBlueprintLibrary.h"


// Sets default values
AInteractable::AInteractable() {
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(FName("Scene Component"));

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("Static mesh"));
	StaticMesh->AttachTo(SceneComponent);

	InteractableWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(FName("Interactable Widget"));
	InteractableWidgetComponent->SetWidgetClass(UInteractableUserWidget::StaticClass());
	InteractableWidgetComponent->AttachTo(StaticMesh);

	OuterTriggerBox = CreateDefaultSubobject<UBoxComponent>(FName("Outer Trigger Box"));
	OuterTriggerBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	OuterTriggerBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	OuterTriggerBox->OnComponentBeginOverlap.AddDynamic(this, &AInteractable::OnOuterBoxOverlapBegin);
	OuterTriggerBox->OnComponentEndOverlap.AddDynamic(this, &AInteractable::OnOuterBoxOverlapEnd);
	OuterTriggerBox->AttachTo(StaticMesh);

	InnerTriggerBox = CreateDefaultSubobject<UBoxComponent>(FName("Inner Trigger Box"));
	InnerTriggerBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	InnerTriggerBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	InnerTriggerBox->OnComponentBeginOverlap.AddDynamic(this, &AInteractable::OnInnerBoxOverlapBegin);
	InnerTriggerBox->OnComponentEndOverlap.AddDynamic(this, &AInteractable::OnInnerBoxOverlapEnd);
	InnerTriggerBox->AttachTo(StaticMesh);

	RootComponent = SceneComponent;
}


void AInteractable::BeginPlay() {
	Super::BeginPlay();

	UInteractableUserWidget* InteractableUserWidget = Cast<UInteractableUserWidget>(InteractableWidgetComponent->GetUserWidgetObject());

	if (IsValid(InteractableUserWidget) == false)
	{
		UE_LOG(LogInventorySystem, Error, TEXT("%s: Interactable Widget is NULL"), *CLASS_FUNCTION);
	}

	HintImage = InteractableUserWidget->GetHintImage();
	TriggerImage = InteractableUserWidget->GetTriggerImage();
	ResourceItemText = InteractableUserWidget->GetText();


	if (IsValid(HintImage) && IsValid(TriggerImage))
	{
		HintImage->SetBrush(UWidgetBlueprintLibrary::MakeBrushFromTexture(InteractableTexture, 50, 50));
		HintImage->SetVisibility(ESlateVisibility::Hidden);

		TriggerImage->SetBrush(UWidgetBlueprintLibrary::MakeBrushFromTexture(TriggerTexture, 50, 50));
		TriggerImage->SetVisibility(ESlateVisibility::Hidden);

		HandleOverlappingActor();
	}
	else
	{
		UE_LOG(LogInventorySystem, Error, TEXT("%s: Error getting interactable images from InteractableWidget"), *CLASS_FUNCTION);
	}

	if (IsValid(ResourceItemText))
	{
		ResourceItemText->SetVisibility(ESlateVisibility::Hidden);
	}
}

void AInteractable::HandleOverlappingActor()
{
	TArray<AActor*> OverlappingActors;

	//Probably some filter for the main character needed
	OuterTriggerBox->GetOverlappingActors(OverlappingActors);
	if (OverlappingActors.Num() > 0)
	{
		HandleOuterBoxBeginOverlap(OverlappingActors[0]);
	}

	InnerTriggerBox->GetOverlappingActors(OverlappingActors);
	if (OverlappingActors.Num() > 0)
	{
		HandleInnerBoxBeginOverlap(OverlappingActors[0]);
	}
}



void AInteractable::ToggleImageVisibility(UImage* Image, bool bVisible)
{
	if (IsValid(Image) == false)
	{
		UE_LOG(LogInventorySystem, Error, TEXT("%s: Image is NULL"), *CLASS_FUNCTION);
		return;
	}

	if (bVisible)
	{
		Image->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		Image->SetVisibility(ESlateVisibility::Hidden);

	}
}

void AInteractable::TogglePlayerControllerEnableInput(APlayerController* Controller, bool bEnableInput)
{
	if (IsValid(Controller) == false)
	{
		UE_LOG(LogInventorySystem, Error, TEXT("%s: Player Controller is nullptr"), *CLASS_FUNCTION);
		return;
	}

	if (bEnableInput)
	{
		EnableInput(Controller);
	}
	else
	{
		DisableInput(Controller);
	}
}

void AInteractable::HandleOuterBoxBeginOverlap(AActor* OverlappedActor)
{
	//Here we can be more specific for the MainCharacter
	ACharacter* Character = Cast<ACharacter>(OverlappedActor);
	if (IsValid(Character) == false)
	{
		UE_LOG(LogInventorySystem, Error, TEXT("%s:Overlapping Character is NULL"), *CLASS_FUNCTION);
		return;
	}

	ToggleImageVisibility(HintImage, true);

	UInventoryComponent* InventoryComponent = Cast<UInventoryComponent>(Character->GetComponentByClass(UInventoryComponent::StaticClass()));
	if (IsValid(InventoryComponent) == false)
	{
		UE_LOG(LogInventorySystem, Error, TEXT("%s: Inventory Component of Overlapping Character is NULL "), *CLASS_FUNCTION);
		return;
	}

	if (!InventoryComponent->CanCarryResources(Resources))
	{
		ResourceItemText->SetVisibility(ESlateVisibility::Visible);
		bShouldShowTriggerImage = false;
	}
}

void AInteractable::HandleInnerBoxBeginOverlap(AActor* OverlappedActor)
{
	ACharacter* Character = Cast<ACharacter>(OverlappedActor);

	if (IsValid(Character) == false)
	{
		UE_LOG(LogInventorySystem, Error, TEXT("%s: Overlapping Character is NULL"), *CLASS_FUNCTION);
		return;
	}
	bShouldListenForTriggerKey = true;

	UWorld* World = GetWorld();

	if (IsValid(World) == false)
	{
		UE_LOG(LogInventorySystem, Error, TEXT("%s: GetWorld() returned NULL"), *CLASS_FUNCTION);
		return;
	}

	TogglePlayerControllerEnableInput(World->GetFirstPlayerController(), true);

	if (bShouldShowTriggerImage)
	{
		ToggleImageVisibility(TriggerImage, true);
	}
}

void AInteractable::OnOuterBoxOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* Other,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	const FHitResult& SweepResult)
{
	HandleOuterBoxBeginOverlap(Other);
}



void AInteractable::OnOuterBoxOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ToggleImageVisibility(HintImage, false);
	ResourceItemText->SetVisibility(ESlateVisibility::Hidden);
}


void AInteractable::OnInnerBoxOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* Other,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	const FHitResult& SweepResult)
{
	HandleInnerBoxBeginOverlap(Other);
}

void AInteractable::OnInnerBoxOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	bShouldListenForTriggerKey = false;

	UWorld* World = GetWorld();
	if (IsValid(World) == false)
	{
		UE_LOG(LogInventorySystem, Error, TEXT("%s: GetWorld() returned NULL"), *CLASS_FUNCTION);
		return;
	}

	TogglePlayerControllerEnableInput(World->GetFirstPlayerController(), false);
	ToggleImageVisibility(TriggerImage, false);
}

void AInteractable::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (bShouldListenForTriggerKey == false)
	{
		return;
	}

	UWorld* World = GetWorld();
	if (IsValid(World) == false)
	{
		UE_LOG(LogInventorySystem, Error, TEXT("%s: GetWordl() returned NULL"), *CLASS_FUNCTION);
		return;
	}

	APlayerController* Controller = World->GetFirstPlayerController();

	if (IsValid(Controller) == false)
	{
		UE_LOG(LogInventorySystem, Error, TEXT("%s: First Player Controller is NULL"), *CLASS_FUNCTION);
		return;
	}

	//TODO: Expose the key as a property
	if (Controller->WasInputKeyJustPressed(EKeys::E))
	{
		OnTriggerKeyPressed(Controller->GetCharacter());
	}
}

void AInteractable::OnTriggerKeyPressed(ACharacter* MainCharacter)
{
	if (IsValid(MainCharacter) == false)
	{
		UE_LOG(LogInventorySystem, Error, TEXT("%s: OverlappedActor is NULL"), *CLASS_FUNCTION);
		return;
	}

	UInventoryComponent* InventoryComponent = Cast<UInventoryComponent>(MainCharacter->GetComponentByClass(UInventoryComponent::StaticClass()));
	if (IsValid(InventoryComponent) == false)
	{
		UE_LOG(LogInventorySystem, Error, TEXT("%s: Error while getting the InventoryComponent"), *CLASS_FUNCTION);
		return;
	}

	if (InventoryComponent->AddResources(Resources))
	{
		Destroy();
	}
}
