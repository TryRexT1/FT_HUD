// Copyright Epic Games, Inc. All Rights Reserved.

#include "FT_HUDGameMode.h"
#include "FT_HUDCharacter.h"
#include "UObject/ConstructorHelpers.h"

AFT_HUDGameMode::AFT_HUDGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
