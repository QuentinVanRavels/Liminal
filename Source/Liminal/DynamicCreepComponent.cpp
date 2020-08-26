// Fill out your copyright notice in the Description page of Project Settings.


#include "DynamicCreepComponent.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"

// Sets default values for this component's properties
UDynamicCreepComponent::UDynamicCreepComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	Owner = GetOwner();
}


// Called when the game starts
void UDynamicCreepComponent::BeginPlay()
{
	Super::BeginPlay();

	TArray<UStaticMeshComponent*> MeshArray;
	Owner->GetComponents<UStaticMeshComponent>(MeshArray);
	UStaticMeshComponent* Mesh = MeshArray[0];
	MaterialDynamic = UMaterialInstanceDynamic::Create(Mesh->GetMaterial(0), this);
}


// Called every frame
void UDynamicCreepComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	
}

