#include "MedievalGameInstance.h"

void UMedievalGameInstance::MarkPickupCollected(const FString& PickupID)
{
	CollectedPickups.Add(PickupID);
	UE_LOG(LogTemp, Log, TEXT("GameInstance: Pickup [%s] marked as collected."), *PickupID);
}

bool UMedievalGameInstance::WasPickupCollected(const FString& PickupID) const
{
	return CollectedPickups.Contains(PickupID);
}

void UMedievalGameInstance::ResetPickups()
{
	CollectedPickups.Empty();
	UE_LOG(LogTemp, Log, TEXT("GameInstance: All pickups reset."));
}
