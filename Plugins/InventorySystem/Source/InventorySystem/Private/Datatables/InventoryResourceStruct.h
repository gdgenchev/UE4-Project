#pragma once

#include "CoreMinimal.h"
#include "Engine/UserDefinedStruct.h"
#include "Engine/DataTable.h"
#include "Engine/Texture2D.h"

#include "InventoryResourceStruct.generated.h"

USTRUCT(BlueprintType)
struct FInventoryResourceStruct : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FName Name;

	UPROPERTY(EditAnywhere)
	uint32 MaxAmount;

	UPROPERTY(EditAnywhere)
	UTexture2D* Texture;
};