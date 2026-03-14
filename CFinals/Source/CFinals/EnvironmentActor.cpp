#include "EnvironmentActor.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Engine/World.h"

AEnvironmentActor::AEnvironmentActor()
{
	PrimaryActorTick.bCanEverTick = true;

	// Root
	// Serves as the actors's attachment anchor
	RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = RootScene;

	// Mesh
	// Sphere mesh attached to the root that bobs and displays the material
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	MeshComp->SetupAttachment(RootComponent);

	// Light
	// light attached to the mesh so it travels during bobbing
	PointLightComp = CreateDefaultSubobject<UPointLightComponent>(TEXT("PointLight"));
	PointLightComp->SetupAttachment(MeshComp);
}

// Responsible for initalizing the dynamic material instance
void AEnvironmentActor::BeginPlay()
{
	Super::BeginPlay();

	// Create the dynamic meterial from slot 0 of the mesh's assigned material
	if (MeshComp && MeshComp->GetMaterial(0))
	{
		DynamicMaterial = MeshComp->CreateDynamicMaterialInstance(0);
	}
}

// Drives runtime systems: transform bobbing, light pulsing, and material glow animation
void AEnvironmentActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Get game time to drive sin/cosine wave calculations
	const float Time = GetWorld()->GetTimeSeconds();

	// Movement
	// Applies sine wave to mesh's Z position to create smooth bobbing
	// frequency and range controlled by MovementSpeed and MovementAmplitude
	if (MeshComp)
	{
		const float Height = FMath::Sin(Time * MovementSpeed) * MovementAmplitude;
		MeshComp->SetRelativeLocation(FVector(0.0f, 0.0f, Height));
	}

	// Lighting
	// Cosine wave is shifted and normalized to 0-1 range
	// LightMinIntensity and LightMaxIntensity set the intensity range
	const float LightAlpha = (FMath::Cos(Time * LightPulseSpeed) + 1.0f) * 0.5f;
	const float LightIntensity = FMath::Lerp(LightMinIntensity, LightMaxIntensity, LightAlpha);

	if (PointLightComp)
	{
		PointLightComp->SetIntensity(LightIntensity);
		// set in the editor via a blueprint variable
		PointLightComp->SetLightColor(LightColor);
	}

	// Material
	// sine wave normalized to 0-1 range
	// GlowMin and GlowMax set the intensity range
	const float GlowAlpha = (FMath::Sin(Time * GlowPulseSpeed) + 1.0f) * 0.5f;
	const float GlowValue = FMath::Lerp(GlowMin, GlowMax, GlowAlpha);

	if (DynamicMaterial)
	{
		DynamicMaterial->SetVectorParameterValue(TEXT("Color"), MaterialColor);
		DynamicMaterial->SetScalarParameterValue(TEXT("Glow"), GlowValue);
	}
}