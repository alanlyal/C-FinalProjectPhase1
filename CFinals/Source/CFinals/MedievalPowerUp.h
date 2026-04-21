#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MedievalPowerUp.generated.h"

UENUM(BlueprintType)
enum class EPowerUpType : uint8
{
	Health  UMETA(DisplayName = "Health Pickup"),
	Stamina UMETA(DisplayName = "Stamina Pickup")
};

UCLASS()
class CFINALS_API AMedievalPowerUp : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMedievalPowerUp();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Config
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PowerUp")
	EPowerUpType PowerUpType = EPowerUpType::Health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PowerUp")
	float RestoreAmount = 30.f;

	// If true, this pickup has already been collected and won't respawn
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PowerUp")
	bool bIsCollected = false;

	// Unique ID used to persist collected state in GameInstance
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PowerUp")
	FString PickupID = TEXT("PowerUp_Default");

	// Spin speed in degrees per second
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PowerUp|Visual")
	float RotationSpeed = 90.f;

	// Bob amplitude in cm
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PowerUp|Visual")
	float BobAmplitude = 15.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PowerUp|Visual")
	float BobSpeed = 2.f;

	// Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UStaticMeshComponent* MeshComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class USphereComponent* CollisionComp;

protected:
	UFUNCTION()
	void OnPickupOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);

private:
	FVector OriginalLocation;
	float   BobTimer = 0.f;

	void ApplyPickupEffect(class AMedievalCharacter* Player);
	void SaveCollectedState();
	bool WasAlreadyCollected() const;
};
