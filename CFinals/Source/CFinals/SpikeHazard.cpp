#include "SpikeHazard.h"
#include "MedievalCharacter.h"
#include "MedievalEnemy.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ASpikeHazard::ASpikeHazard()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpikeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SpikeMesh"));
	RootComponent = SpikeMesh;
	SpikeMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision); // Visual only

	DamageBox = CreateDefaultSubobject<UBoxComponent>(TEXT("DamageBox"));
	DamageBox->SetupAttachment(RootComponent);
	DamageBox->SetCollisionProfileName(TEXT("Trigger"));
	DamageBox->SetBoxExtent(FVector(50.f, 50.f, 60.f)); // Tune in Blueprint to match your mesh

	DamageBox->OnComponentBeginOverlap.AddDynamic(this, &ASpikeHazard::OnDamageBoxBeginOverlap);
	DamageBox->OnComponentEndOverlap.AddDynamic(this, &ASpikeHazard::OnDamageBoxEndOverlap);
}

// Called when the game starts or when spawned
void ASpikeHazard::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ASpikeHazard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	for (auto& Pair : OverlappingActorTimers)
	{
		Pair.Value -= DeltaTime;

		if (Pair.Value <= 0.f)
		{
			ApplyDamageTo(Pair.Key);
			Pair.Value = DamageCooldown;
		}
	}
}

// Overlap Enter
void ASpikeHazard::OnDamageBoxBeginOverlap(
	UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	if (!OtherActor || OtherActor == this) return;

	// Only damage the player or enemies – ignore NPCs and props
	const bool bIsPlayer = OtherActor->IsA<AMedievalCharacter>();
	const bool bIsEnemy = OtherActor->IsA<AMedievalEnemy>();
	if (!bIsPlayer && !bIsEnemy) return;

	ApplyDamageTo(OtherActor);
	OverlappingActorTimers.Add(OtherActor, DamageCooldown);
}

// Overlap Exit
void ASpikeHazard::OnDamageBoxEndOverlap(
	UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	OverlappingActorTimers.Remove(OtherActor);
}

// Apply Damage
void ASpikeHazard::ApplyDamageTo(AActor* Actor)
{
	if (!Actor) return;

	// Guard: don't damage dead actors
	if (AMedievalCharacter* Player = Cast<AMedievalCharacter>(Actor))
	{
		if (Player->bIsDead) return;
	}
	else if (AMedievalEnemy* Enemy = Cast<AMedievalEnemy>(Actor))
	{
		if (Enemy->bIsDead) return;
	}

	// ApplyDamage routes through the AActor TakeDamage override on both classes
	UGameplayStatics::ApplyDamage(
		Actor,
		DamageAmount,
		nullptr,            // No instigator controller – hazard is static
		this,
		UDamageType::StaticClass()
	);
}