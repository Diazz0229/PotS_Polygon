#include "Inventory/POTSItemPickup.h"
#include "Components/StaticMeshComponent.h"
#include "Net/UnrealNetwork.h"

APOTSItemPickup::APOTSItemPickup()
{
    bReplicates = true;
    SetReplicateMovement(false);

    Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    SetRootComponent(Mesh);
    Mesh->SetCollisionProfileName(TEXT("BlockAllDynamic"));
}

bool APOTSItemPickup::Consume()
{
    if (!HasAuthority() || bConsumed)
    {
        return false;
    }

    bConsumed = true;
    return true;
}

void APOTSItemPickup::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(APOTSItemPickup, ItemId);
    DOREPLIFETIME(APOTSItemPickup, Quantity);
    DOREPLIFETIME(APOTSItemPickup, bConsumed);
}
