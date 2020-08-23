// Fill out your copyright notice in the Description page of Project Settings.


#include "PickupDocument.h"
#include "Components/StaticMeshComponent.h"
#include "Blueprint/UserWidget.h"
#include "Runtime/UMG/Public/UMG.h"

// Sets default values
APickupDocument::APickupDocument()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	USceneComponent* SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = SceneComponent;

	DocumentMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Document Mesh"));
	DocumentMesh->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void APickupDocument::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APickupDocument::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

UUserWidget* APickupDocument::GetDocumentContent()
{
	return DocumentContent;
}
