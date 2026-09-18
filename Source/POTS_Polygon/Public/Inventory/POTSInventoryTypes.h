#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "POTSInventoryTypes.generated.h"

class UStaticMesh;
class UTexture2D;

USTRUCT(BlueprintType)
struct POTS_POLYGON_API FPOTSItemDefinition : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
    FName ItemId = NAME_None;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
    FText DisplayName;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item", meta = (ClampMin = "1"))
    int32 MaxStack = 1;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item", meta = (ClampMin = "0.0"))
    float WeightKg = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
    TSoftObjectPtr<UStaticMesh> WorldMesh;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
    TSoftObjectPtr<UTexture2D> Icon;
};

USTRUCT(BlueprintType)
struct POTS_POLYGON_API FPOTSItemStack
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
    FName ItemId = NAME_None;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item", meta = (ClampMin = "1"))
    int32 Quantity = 1;
};

USTRUCT(BlueprintType)
struct POTS_POLYGON_API FPOTSPredictedItem
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "Prediction")
    FGuid RequestId;

    UPROPERTY(BlueprintReadOnly, Category = "Prediction")
    FPOTSItemStack Item;

    UPROPERTY(BlueprintReadOnly, Category = "Prediction")
    bool bServerConfirmed = false;
};
