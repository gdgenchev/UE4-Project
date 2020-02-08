#pragma once

#include "Engine/DataTable.h"
#include "Engine/Texture2D.h"

#include "Resource.generated.h"

UCLASS(BlueprintType, EditInlineNew)
class INVENTORYSYSTEM_API UResource : public UObject
{
    GENERATED_BODY()

public:
    UResource();

protected:

    /*
     * This can be improved with Details Panel Customization.
     * We can read the FNames from the Inventory Datatable and somehow create a dropdown with those names.
     * As currently this is hard for me, I will use FDataTableRowHandle with Inventory Datatable
     * set as default so you can choose the name of the resource from a dropdown.
     * The drawback is that we will store the full info for each item and not just the name.
     * In the future, this class should contain only the FName and the value
     */
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FDataTableRowHandle Resource;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Amount;
public:
    int32 GetAmount();

    FName GetName();
private:
    UDataTable* InventoryDataTable;
};
