#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpikeHazard.generated.h"

class UBoxComponent;
class UStaticMeshComponent;

UCLASS()
class CFINALS_API ASpikeHazard : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASpikeHazard();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Hazard|Components")
	UStaticMeshComponent* SpikeMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Hazard|Components")
	UBoxComponent* DamageBox;

	// Settings
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hazard|Settings")
	float DamageAmount = 20.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hazard|Settings")
	float DamageCooldown = 0.5f;

private:
	TMap<AActor*, float> OverlappingActorTimers;

	UFUNCTION()
	void OnDamageBoxBeginOverlap(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	UFUNCTION()
	void OnDamageBoxEndOverlap(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex);

	void ApplyDamageTo(AActor* Actor);
};
