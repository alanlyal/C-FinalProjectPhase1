#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/PointLightComponent.h"
#include "EnvironmentActor.generated.h"

// EnvironmentActor autonomously controls a transform animation, dynamic lighting, and material grow using C++
// All parameters are exposed to the blueprint for turing within the editor
UCLASS()
class CFINALS_API AEnvironmentActor : public AActor
{
	GENERATED_BODY()

public:
	AEnvironmentActor();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	// Components
	// Serves as the actors's attachment anchor
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CustomComponents")
	USceneComponent* RootScene;

	// Sphere mesh that bobs and displays the material
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CustomComponents")
	UStaticMeshComponent* MeshComp;

	// light which pulses in sync with material glow
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CustomComponents")
	UPointLightComponent* PointLightComp;

	// Movement
	// controls the speed which the mesh bobs up and down
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CustomMovement")
	float MovementSpeed = 2.0f;

	// controls the distance which the mesh travels from the origin point
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CustomMovement")
	float MovementAmplitude = 50.0f;

	// Lighting
	// the colour applied to the lighting
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CustomLighting")
	FLinearColor LightColor = FLinearColor(1.0f, 1.0f, 0.0f, 1.0f);

	// the speed at which the light pulses
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CustomLighting")
	float LightPulseSpeed = 2.0f;

	// the dimmest the light will get during a pulse
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CustomLighting")
	float LightMinIntensity = 400.0f;

	// the brightest the light will get during a pulse
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CustomLighting")
	float LightMaxIntensity = 2400.0f;

	// Material
	// base colour applied to the material
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CustomMaterial")
	FLinearColor MaterialColor = FLinearColor(1.0f, 1.0f, 0.0f, 1.0f);

	// speed of the emissive glow pulses
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CustomMaterial")
	float GlowPulseSpeed = 2.0f;

	// the dimmest the emissive glow will get during a pulse
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CustomMaterial")
	float GlowMin = 1.0f;

	// the brightest the emissive glow will get during a pulse
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CustomMaterial")
	float GlowMax = 8.0f;

private:
	// Dynamic material asset which is created at runtime from the assigned material
	// Allows for per-instance changes without affecting the shared material asset
	UPROPERTY()
	class UMaterialInstanceDynamic* DynamicMaterial;
};