#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "POTSItemPickup.generated.h"

class UStaticMeshComponent;

UCLASS(Blueprintable)
class POTS_POLYGON_API APOTSItemPickup : public AActor
{
    GENERATED_BODY()

public:
    APOTSItemPickup();

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "POTS|Pickup")
    TObjectPtr<UStaticMeshComponent> Mesh;

    UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Replicated, Category = "POTS|Pickup")
    FName ItemId = NAME_None;

    UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Replicated, Category = "POTS|Pickup", meta = (ClampMin = "1"))
    int32 Quantity = 1;

    UFUNCTION(BlueprintPure, Category = "POTS|Pickup")
    bool IsConsumed() const { return bConsumed; }

    bool Consume();

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
    UPROPERTY(Replicated)
    bool bConsumed = false;
};
