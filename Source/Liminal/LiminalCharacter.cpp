// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "LiminalCharacter.h"
#include "LiminalProjectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/InputSettings.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "MotionControllerComponent.h"
#include "XRMotionControllerBase.h" // for FXRMotionControllerBase::RightHandSourceId
#include "UObject/ConstructorHelpers.h"
#include "DrawDebugHelpers.h"

DEFINE_LOG_CATEGORY_STATIC(LogFPChar, Warning, All);

//////////////////////////////////////////////////////////////////////////
// ALiminalCharacter

ALiminalCharacter::ALiminalCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-39.56f, 1.75f, 64.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	CurrentDoor = nullptr;
	CurrentDocument = nullptr;
	IsReadingDocument = false;
}

void ALiminalCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	InfoWidget->AddToViewport();
	InfoWidget->SetVisibility(ESlateVisibility::Hidden);
}

void ALiminalCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FHitResult Hit;
	FVector Start = FirstPersonCameraComponent->GetComponentLocation();

	FVector ForwardVector = FirstPersonCameraComponent->GetForwardVector();
	FVector End = (ForwardVector * 200.f) + Start;

	FCollisionQueryParams CollisionParams;

	//DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 1, 0, 1);

	if (GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, CollisionParams))
	{
		if (Hit.bBlockingHit)
		{	
			InfoWidget->SetVisibility(ESlateVisibility::Visible);
			if (Hit.GetActor()->GetClass()->IsChildOf(ADoor::StaticClass()))
			{
				//InfoWidget->GetWidgetFromName("helpimage")->SetVisibility(ESlateVisibility::Visible);
				CurrentDoor = Cast<ADoor>(Hit.Actor);
			}
			else if (Hit.GetActor()->GetClass()->IsChildOf(APickupDocument::StaticClass()))
			{
				CurrentDocument = Cast<APickupDocument>(Hit.Actor);
			}
		}
	}
	else
	{
		//InfoWidget->GetWidgetFromName("helpimage")->SetVisibility(ESlateVisibility::Hidden);
		InfoWidget->SetVisibility(ESlateVisibility::Hidden);
		CurrentDoor = nullptr;
		CurrentDocument = nullptr;
	}
}

//////////////////////////////////////////////////////////////////////////
// Input

void ALiminalCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// set up gameplay key bindings
	check(PlayerInputComponent);

	// Bind jump events
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAction("Action", IE_Pressed, this, &ALiminalCharacter::OnAction);

	// Bind movement events
	PlayerInputComponent->BindAxis("MoveForward", this, &ALiminalCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ALiminalCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &ALiminalCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &ALiminalCharacter::LookUpAtRate);
}

void ALiminalCharacter::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void ALiminalCharacter::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void ALiminalCharacter::TurnAtRate(float Rate)
{
	if (!IsReadingDocument)
	{
		// calculate delta for this frame from the rate information
		AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
	}
}

void ALiminalCharacter::LookUpAtRate(float Rate)
{
	if (!IsReadingDocument)
	{
		// calculate delta for this frame from the rate information
		AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
	}
}

void ALiminalCharacter::OnAction()
{
	if (CurrentDoor)
	{
		CurrentDoor->ToggleDoor();
	}
	else if (CurrentDocument && !IsReadingDocument)
	{
		IsReadingDocument = true;
		DocumentWidget = CurrentDocument->GetDocumentContent();
		DocumentWidget->AddToViewport();
		CurrentDocument->Destroy();
	}
	else if (IsReadingDocument)
	{
		IsReadingDocument = false;
		DocumentWidget->RemoveFromViewport();
	}
}
