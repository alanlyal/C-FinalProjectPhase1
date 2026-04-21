#include "MedievalEnemy.h"
#include "MedievalCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "AIController.h"
#include "Engine/World.h"

// Sets default values
AMedievalEnemy::AMedievalEnemy()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

// Called when the game starts or when spawned
void AMedievalEnemy::BeginPlay()
{
	Super::BeginPlay();
	CurrentHealth = MaxHealth;

	PlayerTarget = Cast<AMedievalCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
}

// Called every frame
void AMedievalEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsDead) return;

	if (DamageCooldownTimer > 0.f)
	{
		DamageCooldownTimer -= DeltaTime;
	}

	ScanForPlayer();

	if (bIsChasing)
	{
		UpdateChase();
	}
	else
	{
		UpdatePatrol();
	}
}

// Detection

void AMedievalEnemy::ScanForPlayer()
{
	// Don't chase a dead player
	if (!PlayerTarget || PlayerTarget->bIsDead)
	{
		bIsChasing = false;
		return;
	}

	const float DistToPlayer = FVector::Dist(GetActorLocation(), PlayerTarget->GetActorLocation());

	if (!bIsChasing && DistToPlayer <= DetectionRadius)
	{
		bIsChasing = true;
	}
	else if (bIsChasing && DistToPlayer > LoseAggroRadius)
	{
		bIsChasing = false;
		bIsMovingToPatrolPoint = false;
	}
}

// Patrol
void AMedievalEnemy::UpdatePatrol()
{
	if (PatrolPoints.Num() == 0) return;

	AAIController* AIC = Cast<AAIController>(GetController());
	if (!AIC) return;

	GetCharacterMovement()->MaxWalkSpeed = PatrolMoveSpeed;

	AActor* TargetPoint = PatrolPoints[CurrentPatrolIndex];
	if (!TargetPoint) return;

	const float Dist2D = FVector::DistXY(GetActorLocation(), TargetPoint->GetActorLocation());


	if (Dist2D <= PatrolAcceptanceRadius + 50)
	{
		CurrentPatrolIndex = (CurrentPatrolIndex + 1) % PatrolPoints.Num();
		bIsMovingToPatrolPoint = false;
	}

	if (!bIsMovingToPatrolPoint)
	{
		AIC->MoveToActor(PatrolPoints[CurrentPatrolIndex], PatrolAcceptanceRadius);
		bIsMovingToPatrolPoint = true;
	}
}

// Chase
void AMedievalEnemy::UpdateChase()
{
	if (!PlayerTarget) return;

	AAIController* AIC = Cast<AAIController>(GetController());
	if (!AIC) return;

	GetCharacterMovement()->MaxWalkSpeed = ChaseMoveSpeed;

	// Re-issue the move every tick so the path updates as the player moves
	AIC->MoveToActor(PlayerTarget, 0.f);
}

// Contact Damage
void AMedievalEnemy::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	if (bIsDead) return;
	if (DamageCooldownTimer > 0.f) return;

	// only damages AMedievalCharacter
	AMedievalCharacter* HitPlayer = Cast<AMedievalCharacter>(OtherActor);
	if (!HitPlayer || HitPlayer->bIsDead) return;

	UGameplayStatics::ApplyDamage(
		HitPlayer,
		ContactDamageAmount,
		GetController(),
		this,
		UDamageType::StaticClass()
	);

	DamageCooldownTimer = DamageCooldown;
}

// Taking Damage
float AMedievalEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent,
	AController* EventInstigator, AActor* DamageCauser)
{
	if (bIsDead) return 0.f;

	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	CurrentHealth = FMath::Clamp(CurrentHealth - ActualDamage, 0.f, MaxHealth);

	UE_LOG(LogTemp, Log, TEXT("Enemy took %.1f damage. HP: %.1f / %.1f"),
		ActualDamage, CurrentHealth, MaxHealth);

	if (FMath::IsNearlyZero(CurrentHealth))
	{
		Die();
	}

	return ActualDamage;
}

// Death
void AMedievalEnemy::Die()
{
	bIsDead = true;

	// Stop the AI controller from issuing further move requests
	if (AAIController* AIC = Cast<AAIController>(GetController()))
	{
		AIC->StopMovement();
	}

	SetActorEnableCollision(false);
	GetCharacterMovement()->DisableMovement();

	UE_LOG(LogTemp, Warning, TEXT("Enemy died."));

	// 2 second delay
	SetLifeSpan(2.0f);
}