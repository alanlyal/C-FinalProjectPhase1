#include "EnvironmentActor.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Engine/World.h"

AEnvironmentActor::AEnvironmentActor()
{
    PrimaryActorTick.bCanEverTick = true;

    // Root
    RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
    RootComponent = RootScene;

    // Mesh
    MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    MeshComp->SetupAttachment(RootComponent);

    // Light
    PointLightComp = CreateDefaultSubobject<UPointLightComponent>(TEXT("PointLight"));
    PointLightComp->SetupAttachment(MeshComp);
}

void AEnvironmentActor::BeginPlay()
{
    Super::BeginPlay();

    if (MeshComp && MeshComp->GetMaterial(0))
    {
        DynamicMaterial = MeshComp->CreateDynamicMaterialInstance(0);
    }
}

void AEnvironmentActor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    const float Time = GetWorld()->GetTimeSeconds();

    // Movement
    if (MeshComp)
    {
        const float Height = FMath::Sin(Time * MovementSpeed) * MovementAmplitude;
        MeshComp->SetRelativeLocation(FVector(0.0f, 0.0f, Height));
    }

    // Light pulse normalized from 0 to 1
    const float LightAlpha = (FMath::Cos(Time * LightPulseSpeed) + 1.0f) * 0.5f;
    const float LightIntensity = FMath::Lerp(LightMinIntensity, LightMaxIntensity, LightAlpha);

    if (PointLightComp)
    {
        PointLightComp->SetIntensity(LightIntensity);
        PointLightComp->SetLightColor(LightColor);
    }

    // Material glow pulse normalized from 0 to 1
    const float GlowAlpha = (FMath::Sin(Time * GlowPulseSpeed) + 1.0f) * 0.5f;
    const float GlowValue = FMath::Lerp(GlowMin, GlowMax, GlowAlpha);

    if (DynamicMaterial)
    {
        DynamicMaterial->SetVectorParameterValue(TEXT("Color"), MaterialColor);
        DynamicMaterial->SetScalarParameterValue(TEXT("Glow"), GlowValue);
    }
}