// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/RectLightComponent.h"
#include "TimerManager.h"
#include "Components/TimelineComponent.h"
#include "Curves/CurveFloat.h"
#include "Components/BoxComponent.h"
#include "OfficeLight.generated.h"

UCLASS()
class LIMINAL_API AOfficeLight : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AOfficeLight();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UStaticMeshComponent* LightMesh;

	UPROPERTY(VisibleAnywhere, Category = "Light")
	URectLightComponent* LightComponent;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UBoxComponent* LightCollider;

	UPROPERTY(EditAnywhere, Category = "Flicker")
	bool bLightFlicker;

	UPROPERTY(EditAnywhere, Category = "Flicker")
	float MaxFlickerTime;

	UPROPERTY(EditAnywhere, Category = "Flicker")
	float MinFlickerTime;

	UPROPERTY(EditAnywhere, Category = "Flicker")
	int PercentageFlickering;

	UFUNCTION()
	void LightFlicker();

	UFUNCTION()
	void LightFlickerEnd();

	UFUNCTION()
	void StartLightFlicker();

	FTimerHandle LightTimerHandle;

	UPROPERTY(EditAnywhere, Category = "Flicker")
	class UCurveFloat* LightCurve;

	float CurveFloatValue;
	float TimelineValue;
	float CurrentIntensity;
	FTimeline MyTimeline;

	UFUNCTION()
	void OnExitLightArea(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
