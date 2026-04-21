#include "MedievalPowerUp.h"
#include "MedievalCharacter.h"
#include "MedievalGameInstance.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"

// Sets default values
AMedievalPowerUp::AMedievalPowerUp()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
	CollisionComp->InitSphereRadius(80.f);
	CollisionComp->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	SetRootComponent(CollisionComp);

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	MeshComp->SetupAttachment(RootComponent);
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &AMedievalPowerUp::OnPickupOverlap);
}

// Called when the game starts or when spawned
void AMedievalPowerUp::BeginPlay()
{
	Super::BeginPlay();
	OriginalLocation = GetActorLocation();

	// If collected in a previous level, hide immediately
	if (WasAlreadyCollected())
	{
		bIsCollected = true;
		SetActorHiddenInGame(true);
		SetActorEnableCollision(false);
	}
}

// Called every frame
void AMedievalPowerUp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsCollected) return;

	// Spin
	FRotator NewRot = GetActorRotation();
	NewRot.Yaw += RotationSpeed * DeltaTime;
	SetActorRotation(NewRot);

	// Bob up and down
	BobTimer += DeltaTime * BobSpeed;
	FVector NewLoc = OriginalLocation;
	NewLoc.Z += FMath::Sin(BobTimer) * BobAmplitude;
	SetActorLocation(NewLoc);
}

// Overlap/Collection
void AMedievalPowerUp::OnPickupOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	if (bIsCollected) return;

	AMedievalCharacter* Player = Cast<AMedievalCharacter>(OtherActor);
	if (!Player || !Player->IsAlive()) return;

	ApplyPickupEffect(Player);
	SaveCollectedState();

	bIsCollected = true;
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);

	UE_LOG(LogTemp, Log, TEXT("PowerUp [%s] collected by player."), *PickupID);
}

void AMedievalPowerUp::ApplyPickupEffect(AMedievalCharacter* Player)
{
	switch (PowerUpType)
	{
	case EPowerUpType::Health:
		Player->HealHealth(RestoreAmount);
		break;
	case EPowerUpType::Stamina:
		Player->RestoreStamina(RestoreAmount);
		break;
	}
}

// GameInstance persistence
void AMedievalPowerUp::SaveCollectedState()
{
	UMedievalGameInstance* GI = Cast<UMedievalGameInstance>(GetGameInstance());
	if (GI)
	{
		GI->MarkPickupCollected(PickupID);
	}
}

bool AMedievalPowerUp::WasAlreadyCollected() const
{
	UMedievalGameInstance* GI = Cast<UMedievalGameInstance>(GetGameInstance());
	return GI ? GI->WasPickupCollected(PickupID) : false;
}