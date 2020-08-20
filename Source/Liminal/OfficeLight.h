// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/RectLightComponent.h"
#include "TimerManager.h"
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

	UPROPERTY(EditAnywhere, Category = "Light")
	bool bLightFlicker;

	UFUNCTION()
	void LightFlicker();

	FTimerHandle LightTimerHandle;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
