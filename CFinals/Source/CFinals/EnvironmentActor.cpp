#include "EnvironmentActor.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Engine/World.h"

AEnvironmentActor::AEnvironmentActor()
{
	PrimaryActorTick.bCanEverTick = true;
	//root
	RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = RootScene;
	//mesh
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	MeshComp->SetupAttachment(RootComponent);
	//light
	PointLightComp = CreateDefaultSubobject<UPointLightComponent>(TEXT("Pointlight"));
	PointLightComp->SetupAttachment(MeshComp);
}

void AEnvironmentActor::BeginPlay()
{
	Super::BeginPlay();
		if (MeshComp && MeshComp->GetMaterial(0))
		{
			LightMaterial = MeshComp->CreateDynamicMaterialInstance(0);
		}
}

void AEnvironmentActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	float time = GetWorld()->GetTimeSeconds();
	//movement
	float height = FMath::Sin(time * speed) * amplitude;
	MeshComp->SetRelativeLocation(FVector(0.0f, 0.0f, height));
	//light
	float pulse = (FMath::Cos(time * 2.0f) + 1.2f)*2000.0f;
	if (PointLightComp)
	{

		PointLightComp->SetIntensity(pulse);
		PointLightComp->SetLightColor(LightColor);
	}
	//visual
		if (LightMaterial)
		{
			LightMaterial->SetVectorParameterValue(TEXT("Color"), LightColor);
		}

}