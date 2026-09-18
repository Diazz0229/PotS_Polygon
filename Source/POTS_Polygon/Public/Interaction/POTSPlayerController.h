#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "POTSPlayerController.generated.h"

class APOTSItemPickup;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FPOTSPickupPredictionResolved, FGuid, RequestId, bool, bAccepted, FText, FailureReason);

UCLASS(Blueprintable)
class POTS_POLYGON_API APOTSPlayerController : public APlayerController
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintAssignable, Category = "POTS|Interaction")
    FPOTSPickupPredictionResolved OnPickupPredictionResolved;

    UFUNCTION(BlueprintCallable, Category = "POTS|Interaction")
    void RequestPickup(APOTSItemPickup* Pickup);

protected:
    UFUNCTION(Server, Reliable)
    void ServerRequestPickup(APOTSItemPickup* Pickup, FGuid RequestId);

    UFUNCTION(Client, Reliable)
    void ClientPickupResult(FGuid RequestId, bool bAccepted, const FText& FailureReason);

private:
    struct FProcessedPickupRequest
    {
        bool bAccepted = false;
        FText FailureReason;
    };

    TMap<FGuid, TWeakObjectPtr<APOTSItemPickup>> PredictedHiddenPickups;
    TMap<FGuid, FProcessedPickupRequest> ProcessedPickupRequests;

    void CompletePickupRequest(FGuid RequestId, bool bAccepted, const FText& FailureReason);
};
