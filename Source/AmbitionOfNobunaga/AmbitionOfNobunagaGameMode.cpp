// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "AmbitionOfNobunaga.h"
#include "AmbitionOfNobunagaGameMode.h"
#include "AmbitionOfNobunagaPlayerController.h"
#include "AmbitionOfNobunagaCharacter.h"

AAmbitionOfNobunagaGameMode::AAmbitionOfNobunagaGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = AAmbitionOfNobunagaPlayerController::StaticClass();

}