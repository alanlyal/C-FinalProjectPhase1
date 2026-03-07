#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
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
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Root setup") USceneComponent* RootScene;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mesh animation") UStaticMeshComponent* MeshComp;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "light system") UPointLightComponent* PointLightComp;

    //movment
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement settings speed") float speed = 2.0f;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "animation settings amplitude") float amplitude = 50.0f;
    //light default color yellow
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "light color") FLinearColor LightColor = FLinearColor(1.0f, 1.0f, 0.0f, 1.0f);
private:
 UPROPERTY()
 class UMaterialInstanceDynamic* LightMaterial;
};