// Fill out your copyright notice in the Description page of Project Settings.


#include "DynamicCreepComponent.h"
#include "TimerManager.h"
#include "Engine/World.h"
#include "Components/StaticMeshComponent.h"

// Sets default values for this component's properties
UDynamicCreepComponent::UDynamicCreepComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	//PrimaryComponentTick.bCanEverTick = true;

	Owner = GetOwner();
	CreepFactor = 0.f;
}


// Called when the game starts
void UDynamicCreepComponent::BeginPlay()
{
	Super::BeginPlay();

	TArray<UStaticMeshComponent*> MeshArray;
	Owner->GetComponents<UStaticMeshComponent>(MeshArray);
	UStaticMeshComponent* Mesh = MeshArray[0];
	MaterialDynamic = UMaterialInstanceDynamic::Create(Mesh->GetMaterial(0), this);

	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UDynamicCreepComponent::IncreaseCreepyness, 0.01, true, 0.01);
	Mesh->SetMaterial(0, MaterialDynamic);
}

// Called every frame
void UDynamicCreepComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UDynamicCreepComponent::IncreaseCreepyness()
{
	const float incrementValue = 0.01 / ChangeDuration;
	CreepFactor += incrementValue;
	MaterialDynamic->SetScalarParameterValue(FName(TEXT("creepyintensity")), CreepFactor);
	if (CreepFactor >= 1.f)
	{
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	}
}

