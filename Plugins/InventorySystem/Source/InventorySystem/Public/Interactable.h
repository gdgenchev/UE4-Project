// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Character.h"
#include "Components/BoxComponent.h"
#include "Components/SceneComponent.h"
#include "Components/TextBlock.h"
#include "Components/WidgetComponent.h"
#include "Components/Image.h"
#include "Resource.h"
#include "Interactable.generated.h"

/* This class will represent the pickable item, which contains the resources.
 * We might have a Base class Interactable and create a hierarchy.
 * However, for different interactables there are different widgets
 * and the design will become a real mess - calling methods of parent
 * which in term call virtual methods, just to get the wanted behaviour.
 * As this seems an overkill, I will focus just on the items and no other interactables(e.g. artefacts/people/relics/ammo)
 */
UCLASS(Abstract)
class INVENTORYSYSTEM_API AInteractable : public AActor
{
	GENERATED_BODY()
public:
	// Sets default values for this actor's properties
	AInteractable();

protected:
	virtual void BeginPlay() override;

	void HandleOverlappingActor();

	void Tick(float DeltaSeconds) override;

	UPROPERTY()
	USceneComponent* SceneComponent;

	UPROPERTY(VisibleDefaultsOnly)
	UStaticMeshComponent* StaticMesh;

	UPROPERTY(EditDefaultsOnly)
	UBoxComponent* OuterTriggerBox;

	UPROPERTY(EditDefaultsOnly)
	UBoxComponent* InnerTriggerBox;

	UPROPERTY(EditDefaultsOnly)
	UWidgetComponent* InteractableWidgetComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
	UTexture2D* InteractableTexture;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
	UTexture2D* TriggerTexture;

	UFUNCTION()
	void OnOuterBoxOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOuterBoxOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
   void OnInnerBoxOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnInnerBoxOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(EditAnywhere, Instanced, BlueprintReadWrite)
	TArray<UResource*> Resources;
private:
	bool bShouldListenForTriggerKey = false;

	bool bShouldShowTriggerImage = true;

	UPROPERTY()
	UImage* HintImage;

	UPROPERTY()
	UImage* TriggerImage;

	UPROPERTY()
	UTextBlock* ResourceItemText;

	void ToggleImageVisibility(UImage* Image, bool bVisible);

	void HandleInnerBoxBeginOverlap(AActor* OverlappedActor);

	void TogglePlayerControllerEnableInput(APlayerController* Controller, bool bEnableInput);

	void HandleOuterBoxBeginOverlap(AActor* OverlappedActor);

	void OnTriggerKeyPressed(ACharacter* MainCharacter);
};


