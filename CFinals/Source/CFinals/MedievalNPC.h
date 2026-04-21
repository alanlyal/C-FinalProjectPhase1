#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MedievalNPC.generated.h"

UCLASS()
class CFINALS_API AMedievalNPC : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMedievalNPC();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Dialogue/Feedback
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC|Dialogue")
	TArray<FString> DialogueLines;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC|Dialogue")
	float InteractionRadius = 300.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC|Dialogue")
	float DialogueCycleTime = 4.f;

	UFUNCTION(BlueprintCallable, Category = "NPC|Dialogue")
	FString GetCurrentDialogueLine() const;

	UFUNCTION(BlueprintCallable, Category = "NPC|Dialogue")
	bool IsPlayerInRange() const;

	// Waypoint Movement
	// Leave empty for a stationary NPC
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC|Movement")
	TArray<AActor*> Waypoints;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC|Movement")
	float WaypointAcceptRadius = 80.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC|Movement")
	float WaypointWaitTime = 3.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC|Movement")
	float NPCMoveSpeed = 150.f;

	// Widget reference
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC|UI")
	TSubclassOf<class UUserWidget> DialogueWidgetClass;

private:
	int32 CurrentWaypointIndex = 0;
	int32 CurrentDialogueIndex = 0;
	float WaitTimer = 0.f;
	bool  bIsWaiting = false;
	bool  bPlayerWasInRange = false;

	UPROPERTY()
	class UUserWidget* ActiveDialogueWidget;

	void HandleWaypointMovement(float DeltaTime);
	void HandleDialogueTrigger();
	void ShowDialogueWidget();
	void HideDialogueWidget();
	void AdvanceDialogue();

	FTimerHandle DialogueTimerHandle;

};
