// Fill out your copyright notice in the Description page of Project Settings.

#include "PossesedObject.h"
#include "Engine/StaticMesh.h"
#include <Runtime\Engine\Classes\Kismet\KismetMathLibrary.h>

// Sets default values
APossesedObject::APossesedObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RootMesh"));
	RootComponent = StaticMesh;
	StaticMesh->SetCollisionProfileName(TEXT("PhysicsActor"));
	StaticMesh->SetSimulatePhysics(true);
	StaticMesh->SetEnableGravity(true);

	FollowRange = CreateAbstractDefaultSubobject<USphereComponent>((TEXT("FollowRange")));
	FollowRange->InitSphereRadius(600.f);
	FollowRange->SetCollisionProfileName(TEXT("OverlapAll"));
	FollowRange->SetupAttachment(RootComponent);

	FollowRange->OnComponentBeginOverlap.AddDynamic(this, &APossesedObject::OnRangeBeginOverlap);
	FollowRange->OnComponentEndOverlap.AddDynamic(this, &APossesedObject::OnRangeEndOverlap);

	NearbyCharacter = nullptr;

	IsPossesed = true;

	StalkSpeed = 3.f;
}

// Called when the game starts or when spawned
void APossesedObject::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APossesedObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsPossesed && NearbyCharacter != nullptr)
	{
		//check if player is not looking toward object
		FVector ObjectToPlayer = NearbyCharacter->GetActorLocation() - GetActorLocation();
		ObjectToPlayer.Normalize();
		FVector forwardPlayer = NearbyCharacter->GetActorForwardVector();
		ObjectToPlayer.Z = 0.f;
		forwardPlayer.Z = 0.f;

		float angleBetweenVectors = FMath::RadiansToDegrees((acosf(FVector::DotProduct(ObjectToPlayer, forwardPlayer))));

		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("%f"), angleBetweenVectors));

		if (angleBetweenVectors < 80.f)
		{
			FRotator rotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), NearbyCharacter->GetActorLocation());
			SetActorRotation(rotation);

			SetActorLocation(FVector(GetActorLocation() + GetActorForwardVector() * StalkSpeed), true, nullptr, ETeleportType::None);
		}
	}

}

void APossesedObject::OnRangeBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA(ALiminalCharacter::StaticClass()))
	{
		NearbyCharacter = OtherActor;
	}
}

void APossesedObject::OnRangeEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->IsA(ALiminalCharacter::StaticClass()))
	{
		NearbyCharacter = nullptr;
	}
}

