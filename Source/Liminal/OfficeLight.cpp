// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/TimelineComponent.h"
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

	bLightFlicker = false;
	MinFlickerTime = 0.3f;
	MaxFlickerTime = 5.f;

	PercentageFlickering = 50;

}

// Called when the game starts or when spawned
void AOfficeLight::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(LightTimerHandle, this, &AOfficeLight::StartLightFlicker, FMath::RandRange(MinFlickerTime, MaxFlickerTime), false, FMath::RandRange(0.f, 5.f));
	
	int randomNumber = rand() % 100;
	if (randomNumber < PercentageFlickering)
	{
		bLightFlicker = true;
	}
	//init curve
	if (LightCurve && bLightFlicker)
	{
		FOnTimelineFloat TimelineCallback;
		FOnTimelineEvent TimelineFinishedCallback;

		TimelineCallback.BindUFunction(this, FName("LightFlicker"));
		TimelineFinishedCallback.BindUFunction(this, FName("LightFlickerEnd"));
		MyTimeline.AddInterpFloat(LightCurve, TimelineCallback);
		MyTimeline.SetTimelineFinishedFunc(TimelineFinishedCallback);
	}
}

// Called every frame
void AOfficeLight::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	MyTimeline.TickTimeline(DeltaTime);
}

void AOfficeLight::StartLightFlicker()
{
	CurrentIntensity = LightComponent->Intensity;
	MyTimeline.PlayFromStart();
}

void AOfficeLight::LightFlicker()
{
	TimelineValue = MyTimeline.GetPlaybackPosition();
	LightComponent->SetIntensity(CurrentIntensity * LightCurve->GetFloatValue(TimelineValue));
}

void AOfficeLight::LightFlickerEnd()
{
	LightComponent->SetIntensity(CurrentIntensity);
	if (bLightFlicker)
	{
		GetWorldTimerManager().SetTimer(LightTimerHandle, this, &AOfficeLight::StartLightFlicker, FMath::RandRange(MinFlickerTime, MaxFlickerTime), false);
	}
}