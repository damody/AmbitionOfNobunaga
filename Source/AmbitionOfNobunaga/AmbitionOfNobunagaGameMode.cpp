// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "AmbitionOfNobunaga.h"
#include "AmbitionOfNobunagaGameMode.h"
#include "AmbitionOfNobunagaPlayerController.h"
#include "AmbitionOfNobunagaCharacter.h"

AAmbitionOfNobunagaGameMode::AAmbitionOfNobunagaGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = AAmbitionOfNobunagaPlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/AllLogic/Blueprints/TopDownCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}