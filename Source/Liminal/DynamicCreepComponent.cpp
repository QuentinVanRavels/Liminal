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
	NumberOfMaterials = 0;
}


// Called when the game starts
void UDynamicCreepComponent::BeginPlay()
{
	Super::BeginPlay();

	TArray<UStaticMeshComponent*> MeshArray;
	Owner->GetComponents<UStaticMeshComponent>(MeshArray);
	UStaticMeshComponent* Mesh = MeshArray[0];

	if (MaterialDynamic1 == nullptr)
	{
		if (Mesh->GetMaterial(0) != nullptr)
		{
			NumberOfMaterials++;
			MaterialDynamic1 = UMaterialInstanceDynamic::Create(Mesh->GetMaterial(0), this);
			Mesh->SetMaterial(0, MaterialDynamic1);

			if (Mesh->GetMaterial(1) != nullptr)
			{
				NumberOfMaterials++;
				MaterialDynamic2 = UMaterialInstanceDynamic::Create(Mesh->GetMaterial(1), this);
				Mesh->SetMaterial(1, MaterialDynamic2);

				if (Mesh->GetMaterial(2) != nullptr)
				{
					NumberOfMaterials++;
					MaterialDynamic3 = UMaterialInstanceDynamic::Create(Mesh->GetMaterial(2), this);
					Mesh->SetMaterial(2, MaterialDynamic3);
				}
			}
		}
	}

	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UDynamicCreepComponent::IncreaseCreepyness, 0.01, true, 0.01);
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
	if (NumberOfMaterials >= 1)
	{
		MaterialDynamic1->SetScalarParameterValue(FName(TEXT("creepy intensity")), CreepFactor);
		if (NumberOfMaterials >= 2)
		{
			MaterialDynamic2->SetScalarParameterValue(FName(TEXT("creepy intensity")), CreepFactor);
			if (NumberOfMaterials >= 3)
			{
				MaterialDynamic3->SetScalarParameterValue(FName(TEXT("creepy intensity")), CreepFactor);
			}
		}
	}

	if (CreepFactor >= 1.f)
	{
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	}
}

