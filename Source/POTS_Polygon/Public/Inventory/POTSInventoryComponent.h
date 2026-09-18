#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Inventory/POTSInventoryTypes.h"
#include "POTSInventoryComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPOTSInventoryChanged);

UCLASS(ClassGroup = (POTS), BlueprintType, Blueprintable, meta = (BlueprintSpawnableComponent))
class POTS_POLYGON_API UPOTSInventoryComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UPOTSInventoryComponent();

    UPROPERTY(ReplicatedUsing = OnRep_AuthoritativeItems, BlueprintReadOnly, Category = "POTS|Inventory")
    TArray<FPOTSItemStack> AuthoritativeItems;

    UPROPERTY(Transient, BlueprintReadOnly, Category = "POTS|Inventory|Prediction")
    TArray<FPOTSPredictedItem> PredictedItems;

    UPROPERTY(BlueprintAssignable, Category = "POTS|Inventory")
    FPOTSInventoryChanged OnInventoryChanged;

    UFUNCTION(BlueprintCallable, Category = "POTS|Inventory")
    void AddPredictedItem(const FGuid& RequestId, FName ItemId, int32 Quantity);

    UFUNCTION(BlueprintCallable, Category = "POTS|Inventory")
    void ResolvePredictedItem(const FGuid& RequestId, bool bAccepted);

    UFUNCTION(BlueprintPure, Category = "POTS|Inventory")
    int32 GetDisplayQuantity(FName ItemId) const;

    void AddAuthoritativeItem(FName ItemId, int32 Quantity);

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
    UFUNCTION()
    void OnRep_AuthoritativeItems();

    void AddOrMergeStack(TArray<FPOTSItemStack>& Items, FName ItemId, int32 Quantity);
};
