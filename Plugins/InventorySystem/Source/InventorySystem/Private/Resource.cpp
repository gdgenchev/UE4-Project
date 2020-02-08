#include "Resource.h"
#include "UObject/ConstructorHelpers.h"
#include "Datatables/InventoryResourceStruct.h"

UResource::UResource() {
    static ConstructorHelpers::FObjectFinder<UDataTable> InventoryItemsDataObject(
        TEXT("DataTable'/InventorySystem/Blueprints/DataTables/BP_InventoryResourcesDatatable.BP_InventoryResourcesDatatable'"));
    if ( InventoryItemsDataObject.Succeeded()) {
		InventoryDataTable = InventoryItemsDataObject.Object;
		Resource.DataTable = InventoryDataTable;
    }

    Amount = 0;
}

FName UResource::GetName() {
    return Resource.RowName;
}

int32 UResource::GetAmount() {
    return Amount;
}