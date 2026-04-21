#include "MedievalNPC.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Navigation/PathFollowingComponent.h"
#include "NavigationSystem.h"

// Sets default values
AMedievalNPC::AMedievalNPC()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Default dialogue lines
	DialogueLines.Add(TEXT("Greetings, traveller."));
	DialogueLines.Add(TEXT("Many brave souls have ventured there. Few return."));
	DialogueLines.Add(TEXT("Stay on the path and you may survive."));
}

// Called when the game starts or when spawned
void AMedievalNPC::BeginPlay()
{
	Super::BeginPlay();

	if (GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = NPCMoveSpeed;
	}

	// Kick off dialogue cycling timer
	if (DialogueLines.Num() > 1)
	{
		GetWorldTimerManager().SetTimer(
			DialogueTimerHandle,
			this,
			&AMedievalNPC::AdvanceDialogue,
			DialogueCycleTime,
			true  // looping
		);
	}
}

// Called every frame
void AMedievalNPC::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	HandleDialogueTrigger();

	if (Waypoints.Num() > 0)
	{
		HandleWaypointMovement(DeltaTime);
	}
}

// Waypoint Movement
void AMedievalNPC::HandleWaypointMovement(float DeltaTime)
{
	if (bIsWaiting)
	{
		WaitTimer -= DeltaTime;
		if (WaitTimer <= 0.f)
		{
			bIsWaiting = false;
		}
		return;
	}

	AActor* Target = Waypoints[CurrentWaypointIndex];
	if (!Target) return;

	FVector ToTarget = Target->GetActorLocation() - GetActorLocation();
	ToTarget.Z = 0.f;

	if (ToTarget.Size() <= WaypointAcceptRadius)
	{
		// Reached waypoint — wait, then advance
		bIsWaiting = true;
		WaitTimer = WaypointWaitTime;
		CurrentWaypointIndex = (CurrentWaypointIndex + 1) % Waypoints.Num();
	}
	else
	{
		// Move toward waypoint
		FVector Direction = ToTarget.GetSafeNormal();
		AddMovementInput(Direction, 1.f);
	}
}

// Dialogue
void AMedievalNPC::HandleDialogueTrigger()
{
	bool bInRange = IsPlayerInRange();

	if (bInRange && !bPlayerWasInRange)
	{
		ShowDialogueWidget();
	}
	else if (!bInRange && bPlayerWasInRange)
	{
		HideDialogueWidget();
	}

	bPlayerWasInRange = bInRange;
}

bool AMedievalNPC::IsPlayerInRange() const
{
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (!PC || !PC->GetPawn()) return false;

	float Dist = FVector::Dist(GetActorLocation(), PC->GetPawn()->GetActorLocation());
	return Dist <= InteractionRadius;
}

FString AMedievalNPC::GetCurrentDialogueLine() const
{
	if (DialogueLines.Num() == 0) return TEXT("");
	return DialogueLines[CurrentDialogueIndex];
}

void AMedievalNPC::AdvanceDialogue()
{
	if (DialogueLines.Num() == 0) return;
	CurrentDialogueIndex = (CurrentDialogueIndex + 1) % DialogueLines.Num();

	UE_LOG(LogTemp, Log, TEXT("NPC dialogue: %s"), *GetCurrentDialogueLine());
}

void AMedievalNPC::ShowDialogueWidget()
{
	if (!DialogueWidgetClass) return;

	if (!ActiveDialogueWidget)
	{
		APlayerController* PC = GetWorld()->GetFirstPlayerController();
		if (!PC) return;

		ActiveDialogueWidget = CreateWidget<UUserWidget>(PC, DialogueWidgetClass);
	}

	if (ActiveDialogueWidget && !ActiveDialogueWidget->IsInViewport())
	{
		ActiveDialogueWidget->AddToViewport();
		UE_LOG(LogTemp, Log, TEXT("NPC dialogue widget shown."));
	}
}

void AMedievalNPC::HideDialogueWidget()
{
	if (ActiveDialogueWidget && ActiveDialogueWidget->IsInViewport())
	{
		ActiveDialogueWidget->RemoveFromParent();
		UE_LOG(LogTemp, Log, TEXT("NPC dialogue widget hidden."));
	}
}

