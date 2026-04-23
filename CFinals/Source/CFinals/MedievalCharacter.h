#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "MedievalCharacter.generated.h"

class UInputAction;
class UUserWidget;

UCLASS()
class CFINALS_API AMedievalCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMedievalCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// HUD
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UUserWidget> PlayerHUDClass;

	UPROPERTY()
	UUserWidget* PlayerHUD;

public:

	// UI Getters
	UFUNCTION(BlueprintPure, Category = "UI")
	float GetHealthPercent() const;

	UFUNCTION(BlueprintPure, Category = "UI")
	float GetStaminaPercent() const;

	UFUNCTION(BlueprintPure, Category = "UI")
	float GetCurrentHealth() const { return CurrentHealth; }

	UFUNCTION(BlueprintPure, Category = "UI")
	float GetMaxHealth() const { return MaxHealth; }

	UFUNCTION(BlueprintPure, Category = "UI")
	float GetCurrentStamina() const { return CurrentStamina; }

	UFUNCTION(BlueprintPure, Category = "UI")
	float GetMaxStamina() const { return MaxStamina; }

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Stats
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float MaxHealth = 100.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats")
	float CurrentHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float MaxStamina = 100.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats")
	float CurrentStamina;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float StaminaRegenRate = 5.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	bool bIsDead = false;

	// Movement
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float WalkSpeed = 400.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float SprintSpeed = 700.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float SprintStaminaCost = 10.f;

	// Input
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	class UInputAction* SprintAction;

	//Combat
	UFUNCTION(BlueprintCallable, Category = "Combat")
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent,
		AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void HealHealth(float Amount);

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void RestoreStamina(float Amount);

	UFUNCTION(BlueprintPure, Category = "Combat")
	bool IsAlive() const { return !bIsDead; }

private:
	bool bIsSprinting = false;

	void StartSprint();
	void StopSprint();
	void HandleStamina(float DeltaTime);
	void Die();
};
