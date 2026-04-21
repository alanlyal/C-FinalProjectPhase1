#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MedievalEnemy.generated.h"

class UAIMovementComponent;

UCLASS()
class CFINALS_API AMedievalEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMedievalEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

	// Health
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|Stats")
	float MaxHealth = 100.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Enemy|Stats")
	float CurrentHealth;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Enemy|Stats")
	bool bIsDead = false;

	UFUNCTION(BlueprintCallable, Category = "Enemy|Combat")
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent,
		AController* EventInstigator, AActor* DamageCauser) override;

	// Contact Damage
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|Combat")
	float ContactDamageAmount = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|Combat")
	float DamageCooldown = 1.0f;

	// Patrol
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|Patrol")
	TArray<AActor*> PatrolPoints;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|Patrol")
	float PatrolAcceptanceRadius = 150.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|Patrol")
	float PatrolMoveSpeed = 300.f;

	// Detection/Chasing
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|Detection")
	float DetectionRadius = 800.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|Detection")
	float LoseAggroRadius = 1200.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|Detection")
	float ChaseMoveSpeed = 500.f;

private:
	float DamageCooldownTimer = 0.f;
	bool bIsChasing = false;
	bool bIsMovingToPatrolPoint = false;
	int32 CurrentPatrolIndex = 0;

	class AMedievalCharacter* PlayerTarget = nullptr;

	void ScanForPlayer();
	void UpdatePatrol();
	void UpdateChase();
	void Die();
};
