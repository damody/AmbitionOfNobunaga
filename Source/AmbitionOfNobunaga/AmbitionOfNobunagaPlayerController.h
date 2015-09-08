// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/PlayerController.h"
#include "AmbitionOfNobunagaPlayerController.generated.h"

class ARTS_HUD;

UCLASS()
class AAmbitionOfNobunagaPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AAmbitionOfNobunagaPlayerController();

protected:
	ARTS_HUD* Hud;

	virtual void BeginPlay() override;

	/** True if the controlled character should navigate to the mouse cursor. */
	uint32 bMoveToMouseCursor : 1;
	uint32 bMouseRButton : 1;
	uint32 bMouseLButton : 1;

	// Begin PlayerController interface
	virtual void PlayerTick(float DeltaTime) override;
	virtual void SetupInputComponent() override;
	// End PlayerController interface

	UFUNCTION(BlueprintImplementableEvent)
	void MouseCursorTouch3DLocation(const FVector Location);

	/** Navigate player to the current mouse cursor location. */
	void MoveToMouseCursor();

	/** Navigate player to the current touch location. */
	void MoveToTouchLocation(const ETouchIndex::Type FingerIndex, const FVector Location);
	
	/** Navigate player to the given world location. */
	void SetNewMoveDestination(const FVector DestLocation);

	FVector2D GetMouseScreenPosition();

	/** Input handlers for SetDestination action. */
	void OnSetDestinationPressed();
	void OnSetDestinationReleased();

	void OnMouseRButtonPressed();
	void OnMouseRButtonReleased();

	void OnMouseLButtonPressed();
	void OnMouseLButtonReleased();
};


