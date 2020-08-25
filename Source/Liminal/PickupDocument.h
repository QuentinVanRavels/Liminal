// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PickupDocument.generated.h"

UCLASS()
class LIMINAL_API APickupDocument : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickupDocument();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UStaticMeshComponent* DocumentMesh;

	UPROPERTY(EditAnywhere, Category = "Document")
	UUserWidget* DocumentContent;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UUserWidget* GetDocumentContent();

};
