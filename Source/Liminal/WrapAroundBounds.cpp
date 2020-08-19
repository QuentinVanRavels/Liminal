// Fill out your copyright notice in the Description page of Project Settings.
//Bounds will automatically position themselves in the middle of the terrain

#include "WrapAroundBounds.h"

// Sets default values
AWrapAroundBounds::AWrapAroundBounds()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Boundries = CreateDefaultSubobject<UBoxComponent>(TEXT("Boundries"));
	Boundries->SetGenerateOverlapEvents(true);
	Boundries->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);

	Boundries->OnComponentEndOverlap.AddDynamic(this, &AWrapAroundBounds::OnWrapAround);
}

// Called when the game starts or when spawned
void AWrapAroundBounds::BeginPlay()
{
	Super::BeginPlay();

	Boundries->SetBoxExtent(Size);
	SetActorLocation(FVector(0.f, 0.f, 0.f));
}

// Called every frame
void AWrapAroundBounds::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWrapAroundBounds::OnWrapAround(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) //bool bFromSweep, const FHitResult& SweepResult)
{
	//check where overlap happened, then teleport
	//only teleport if actor is walking towards the collider
	FVector Location = OtherActor->GetActorLocation();
	FVector ActorVelocity = OtherActor->GetVelocity();
	if (FMath::Abs(Location.X) > FMath::Abs(Location.Y))
	{
		if ((Location.X > 0.f && ActorVelocity.X > 0.f) || (Location.X < 0.f && ActorVelocity.X < 0.f))
		{
			OtherActor->SetActorLocation(FVector(-Location.X, Location.Y, Location.Z));
		}
	}
	else
	{
		if ((Location.Y > 0.f && ActorVelocity.Y > 0.f) || (Location.Y < 0.f && ActorVelocity.Y < 0.f))
		{
			OtherActor->SetActorLocation(FVector(Location.X, -Location.Y, Location.Z));
		}
	}
}

