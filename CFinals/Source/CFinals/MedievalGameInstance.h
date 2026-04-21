#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MedievalGameInstance.generated.h"

/**
 * Persists data across level transitions
 * Tracks which power-ups have been collected
 */
UCLASS()
class CFINALS_API UMedievalGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Persistence")
	void MarkPickupCollected(const FString& PickupID);

	UFUNCTION(BlueprintPure, Category = "Persistence")
	bool WasPickupCollected(const FString& PickupID) const;

	UFUNCTION(BlueprintCallable, Category = "Persistence")
	void ResetPickups();

private:
	TSet<FString> CollectedPickups;
};
