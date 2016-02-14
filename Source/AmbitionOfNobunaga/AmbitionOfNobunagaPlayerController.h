// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/PlayerController.h"
#include "Engine/EngineBaseTypes.h"
#include "AmbitionOfNobunagaPlayerController.generated.h"

class ARTS_HUD;
class AHeroCharacter;
class AEquipment;



UCLASS()
class AAmbitionOfNobunagaPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AAmbitionOfNobunagaPlayerController();

protected:
	ARTS_HUD* Hud;

	virtual void BeginPlay() override;
	
	virtual bool InputKey(FKey Key, EInputEvent EventType, float AmountDepressed, bool bGamepad) override;

	/** True if the controlled character should navigate to the mouse cursor. */
	uint32 bMoveToMouseCursor : 1;
	uint32 bMouseRButton : 1;
	uint32 bMouseLButton : 1;
	UPrimitiveComponent* ClickedPrimitive;


	// Begin PlayerController interface
	virtual void PlayerTick(float DeltaTime) override;
	virtual void SetupInputComponent() override;
	// End PlayerController interface
	
public:
		
	// 當前滑鼠坐標
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RTS")
	FVector2D CurrentMouseXY;

	// 地板Actor名 滑鼠LineTrace用
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RTS")
	FString FloorActorName;

	/** Navigate player to the given world location. */
	
// 	UFUNCTION(Server, WithValidation, Reliable, BlueprintCallable, Category = "RTS")
// 	void AddHeroToMoveQueue(const FVector DestLocation, const TArray<AHeroCharacter*>& heros);


	FVector2D GetMouseScreenPosition();

	void OnMouseRButtonPressed1();
	void OnMouseRButtonPressed2();
	void OnMouseRButtonReleased();

	void OnMouseLButtonPressed1();
	void OnMouseLButtonPressed2();
	void OnMouseLButtonReleased();
};


