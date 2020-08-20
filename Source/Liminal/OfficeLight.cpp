// Fill out your copyright notice in the Description page of Project Settings.

#include "OfficeLight.h"

// Sets default values
AOfficeLight::AOfficeLight()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	USceneComponent* SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = SceneComponent;
	LightMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LightMesh"));
	LightMesh->SetupAttachment(RootComponent);
	LightComponent = CreateDefaultSubobject<URectLightComponent>(TEXT("LightSource"));
	LightComponent->SetupAttachment(RootComponent);

	bLightFlicker = true;
}

// Called when the game starts or when spawned
void AOfficeLight::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(LightTimerHandle, this, &AOfficeLight::LightFlicker, FMath::RandRange(0.f, 5.f), true, FMath::RandRange(0.f, 5.f));
	
}

// Called every frame
void AOfficeLight::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AOfficeLight::LightFlicker()
{
	LightComponent->ToggleVisibility();
}

