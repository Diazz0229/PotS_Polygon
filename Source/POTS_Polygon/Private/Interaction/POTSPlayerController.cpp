#include "Interaction/POTSPlayerController.h"
#include "Inventory/POTSInventoryComponent.h"
#include "Inventory/POTSItemPickup.h"
#include "GameFramework/Pawn.h"

void APOTSPlayerController::RequestPickup(APOTSItemPickup* Pickup)
{
    if (!IsLocalController() || !IsValid(Pickup) || Pickup->IsConsumed())
    {
        return;
    }

    UPOTSInventoryComponent* Inventory = GetPawn() ? GetPawn()->FindComponentByClass<UPOTSInventoryComponent>() : nullptr;
    if (!Inventory)
    {
        return;
    }

    const FGuid RequestId = FGuid::NewGuid();
    Inventory->AddPredictedItem(RequestId, Pickup->ItemId, Pickup->Quantity);
    Pickup->SetActorHiddenInGame(true);
    PredictedHiddenPickups.Add(RequestId, Pickup);
    ServerRequestPickup(Pickup, RequestId);
}

void APOTSPlayerController::ServerRequestPickup_Implementation(APOTSItemPickup* Pickup, const FGuid RequestId)
{
    // P0 policy: the client validates interaction range, LOS, capacity and prerequisites.
    // The server only serializes the shared world state and rejects stale/duplicate targets.
    if (const FProcessedPickupRequest* PreviousResult = ProcessedPickupRequests.Find(RequestId))
    {
        ClientPickupResult(RequestId, PreviousResult->bAccepted, PreviousResult->FailureReason);
        return;
    }

    if (!IsValid(Pickup) || Pickup->IsConsumed())
    {
        CompletePickupRequest(RequestId, false, FText::FromString(TEXT("Предмет уже недоступен")));
        return;
    }

    UPOTSInventoryComponent* Inventory = GetPawn() ? GetPawn()->FindComponentByClass<UPOTSInventoryComponent>() : nullptr;
    if (!Inventory || !Pickup->Consume())
    {
        CompletePickupRequest(RequestId, false, FText::FromString(TEXT("Подбор не выполнен")));
        return;
    }

    Inventory->AddAuthoritativeItem(Pickup->ItemId, Pickup->Quantity);
    CompletePickupRequest(RequestId, true, FText::GetEmpty());
    Pickup->Destroy();
}

void APOTSPlayerController::CompletePickupRequest(const FGuid RequestId, const bool bAccepted, const FText& FailureReason)
{
    FProcessedPickupRequest& Result = ProcessedPickupRequests.FindOrAdd(RequestId);
    Result.bAccepted = bAccepted;
    Result.FailureReason = FailureReason;
    ClientPickupResult(RequestId, bAccepted, FailureReason);
}

void APOTSPlayerController::ClientPickupResult_Implementation(const FGuid RequestId, const bool bAccepted, const FText& FailureReason)
{
    UPOTSInventoryComponent* Inventory = GetPawn() ? GetPawn()->FindComponentByClass<UPOTSInventoryComponent>() : nullptr;
    if (Inventory)
    {
        Inventory->ResolvePredictedItem(RequestId, bAccepted);
    }

    if (!bAccepted)
    {
        if (const TWeakObjectPtr<APOTSItemPickup>* PredictedPickup = PredictedHiddenPickups.Find(RequestId))
        {
            if (PredictedPickup->IsValid())
            {
                PredictedPickup->Get()->SetActorHiddenInGame(false);
            }
        }
    }

    PredictedHiddenPickups.Remove(RequestId);
    OnPickupPredictionResolved.Broadcast(RequestId, bAccepted, FailureReason);
}
