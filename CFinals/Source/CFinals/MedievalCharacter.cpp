#include "MedievalCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputComponent.h"
#include "Blueprint/UserWidget.h"

// Sets default values
AMedievalCharacter::AMedievalCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMedievalCharacter::BeginPlay()
{
	Super::BeginPlay();

	CurrentHealth = MaxHealth * 0.3f;
	CurrentStamina = MaxStamina * 0.3f;

	if (GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	}

	if (PlayerHUDClass)
	{
		PlayerHUD = CreateWidget<UUserWidget>(GetWorld(), PlayerHUDClass);
		if (PlayerHUD)
		{
			PlayerHUD->AddToViewport();
		}
	}

}

// Called every frame
void AMedievalCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	HandleStamina(DeltaTime);
}

float AMedievalCharacter::GetHealthPercent() const
{
	if (MaxHealth <= 0.f) return 0.f;
	return CurrentHealth / MaxHealth;
}

float AMedievalCharacter::GetStaminaPercent() const
{
	if (MaxStamina <= 0.f) return 0.f;
	return CurrentStamina / MaxStamina;
}

void AMedievalCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (SprintAction)
		{
			EIC->BindAction(SprintAction, ETriggerEvent::Started, this, &AMedievalCharacter::StartSprint);
			EIC->BindAction(SprintAction, ETriggerEvent::Completed, this, &AMedievalCharacter::StopSprint);
		}
	}
}

// Sprinting
void AMedievalCharacter::StartSprint()
{
	if (CurrentStamina > 0.f && IsAlive())
	{
		bIsSprinting = true;
		if (GetCharacterMovement())
		{
			GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
		}
	}
}

void AMedievalCharacter::StopSprint()
{
	bIsSprinting = false;
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	}
}

// Stamina
void AMedievalCharacter::HandleStamina(float DeltaTime)
{
	if (bIsSprinting)
	{
		CurrentStamina = FMath::Clamp(CurrentStamina - SprintStaminaCost * DeltaTime, 0.f, MaxStamina);
		if (FMath::IsNearlyZero(CurrentStamina))
		{
			StopSprint();
		}
	}
	else
	{
		CurrentStamina = FMath::Clamp(CurrentStamina + StaminaRegenRate * DeltaTime, 0.f, MaxStamina);
	}
}

// Combat
float AMedievalCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent,
	AController* EventInstigator, AActor* DamageCauser)
{
	if (bIsDead) return 0.f;

	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	CurrentHealth = FMath::Clamp(CurrentHealth - ActualDamage, 0.f, MaxHealth);

	UE_LOG(LogTemp, Warning, TEXT("Player took %.1f damage. Health: %.1f / %.1f"),
		ActualDamage, CurrentHealth, MaxHealth);

	if (FMath::IsNearlyZero(CurrentHealth))
	{
		Die();
	}

	return ActualDamage;
}

void AMedievalCharacter::HealHealth(float Amount)
{
	if (bIsDead) return;
	CurrentHealth = FMath::Clamp(CurrentHealth + Amount, 0.f, MaxHealth);
	UE_LOG(LogTemp, Log, TEXT("Player healed %.1f. Health: %.1f / %.1f"), Amount, CurrentHealth, MaxHealth);
}

void AMedievalCharacter::RestoreStamina(float Amount)
{
	if (bIsDead) return;
	CurrentStamina = FMath::Clamp(CurrentStamina + Amount, 0.f, MaxStamina);
	UE_LOG(LogTemp, Log, TEXT("Player restored %.1f stamina. Stamina: %.1f / %.1f"), Amount, CurrentStamina, MaxStamina);
}

void AMedievalCharacter::Die()
{
	bIsDead = true;
	StopSprint();

	// Ragdoll on death
	GetMesh()->SetSimulatePhysics(true);
	GetCharacterMovement()->DisableMovement();

	UE_LOG(LogTemp, Warning, TEXT("Player has died."));
}