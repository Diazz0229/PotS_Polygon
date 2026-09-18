#include "Inventory/POTSInventoryComponent.h"
#include "Net/UnrealNetwork.h"

UPOTSInventoryComponent::UPOTSInventoryComponent()
{
    SetIsReplicatedByDefault(true);
}

void UPOTSInventoryComponent::AddPredictedItem(const FGuid& RequestId, const FName ItemId, const int32 Quantity)
{
    if (!RequestId.IsValid() || ItemId.IsNone() || Quantity <= 0)
    {
        return;
    }

    FPOTSPredictedItem& Prediction = PredictedItems.AddDefaulted_GetRef();
    Prediction.RequestId = RequestId;
    Prediction.Item.ItemId = ItemId;
    Prediction.Item.Quantity = Quantity;
    OnInventoryChanged.Broadcast();
}

void UPOTSInventoryComponent::ResolvePredictedItem(const FGuid& RequestId, const bool bAccepted)
{
    const int32 Index = PredictedItems.IndexOfByPredicate([&RequestId](const FPOTSPredictedItem& Item)
    {
        return Item.RequestId == RequestId;
    });

    if (Index == INDEX_NONE)
    {
        return;
    }

    if (bAccepted)
    {
        PredictedItems[Index].bServerConfirmed = true;
    }
    else
    {
        PredictedItems.RemoveAt(Index);
    }

    OnInventoryChanged.Broadcast();
}

int32 UPOTSInventoryComponent::GetDisplayQuantity(const FName ItemId) const
{
    int32 Result = 0;

    for (const FPOTSItemStack& Stack : AuthoritativeItems)
    {
        if (Stack.ItemId == ItemId)
        {
            Result += Stack.Quantity;
        }
    }

    for (const FPOTSPredictedItem& Prediction : PredictedItems)
    {
        if (Prediction.Item.ItemId == ItemId)
        {
            Result += Prediction.Item.Quantity;
        }
    }

    return Result;
}

void UPOTSInventoryComponent::AddAuthoritativeItem(const FName ItemId, const int32 Quantity)
{
    if (!GetOwner() || !GetOwner()->HasAuthority() || ItemId.IsNone() || Quantity <= 0)
    {
        return;
    }

    AddOrMergeStack(AuthoritativeItems, ItemId, Quantity);
    OnInventoryChanged.Broadcast();
}

void UPOTSInventoryComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME_CONDITION(UPOTSInventoryComponent, AuthoritativeItems, COND_OwnerOnly);
}

void UPOTSInventoryComponent::OnRep_AuthoritativeItems()
{
    PredictedItems.RemoveAll([](const FPOTSPredictedItem& Prediction)
    {
        return Prediction.bServerConfirmed;
    });
    OnInventoryChanged.Broadcast();
}

void UPOTSInventoryComponent::AddOrMergeStack(TArray<FPOTSItemStack>& Items, const FName ItemId, const int32 Quantity)
{
    FPOTSItemStack* ExistingStack = Items.FindByPredicate([ItemId](const FPOTSItemStack& Stack)
    {
        return Stack.ItemId == ItemId;
    });

    if (ExistingStack)
    {
        ExistingStack->Quantity += Quantity;
        return;
    }

    FPOTSItemStack& NewStack = Items.AddDefaulted_GetRef();
    NewStack.ItemId = ItemId;
    NewStack.Quantity = Quantity;
}
