// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "LiminalGameMode.h"
#include "LiminalHUD.h"
#include "LiminalCharacter.h"
#include "UObject/ConstructorHelpers.h"

ALiminalGameMode::ALiminalGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	//HUDClass = ALiminalHUD::StaticClass();
}
