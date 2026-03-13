#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/PointLightComponent.h"
#include "EnvironmentActor.generated.h"

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
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CustomComponents")
    USceneComponent* RootScene;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CustomComponents")
    UStaticMeshComponent* MeshComp;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CustomComponents")
    UPointLightComponent* PointLightComp;

    // Movement
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CustomMovement")
    float MovementSpeed = 2.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CustomMovement")
    float MovementAmplitude = 50.0f;

    // Lighting
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CustomLighting")
    FLinearColor LightColor = FLinearColor(1.0f, 1.0f, 0.0f, 1.0f);

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CustomLighting")
    float LightPulseSpeed = 2.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CustomLighting")
    float LightMinIntensity = 400.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CustomLighting")
    float LightMaxIntensity = 2400.0f;

    // Material
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CustomMaterial")
    FLinearColor MaterialColor = FLinearColor(1.0f, 1.0f, 0.0f, 1.0f);

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CustomMaterial")
    float GlowPulseSpeed = 2.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CustomMaterial")
    float GlowMin = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CustomMaterial")
    float GlowMax = 8.0f;

private:
    UPROPERTY()
    class UMaterialInstanceDynamic* DynamicMaterial;
};