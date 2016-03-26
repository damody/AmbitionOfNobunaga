// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/PlayerController.h"
#include "Engine/EngineBaseTypes.h"
#include "HeroActionx.h"
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

	// 有註冊的鍵盤事件
	TArray<FKey> SkillMapping;

	/** Navigate player to the given world location. */

	void CharacterMoveImpl(AHeroCharacter* hero, const FVector& action);
	
	UFUNCTION(Server, WithValidation, Reliable, Category = "AONGameState")
	void CharacterMove(AHeroCharacter* hero, const FVector& pos);
	
	UFUNCTION(Server, WithValidation, Reliable, BlueprintCallable, Category = "AONGameState")
	void CharacterStopMove(AHeroCharacter* hero);

	UFUNCTION(Server, WithValidation, Reliable, BlueprintCallable, Category = "AONGameState")
	void HeroUseSkill(AHeroCharacter* hero, int32 index, const FVector& VFaceTo, const FVector& pos);

	void SetHeroActionImpl(AHeroCharacter* hero, const FHeroAction& action);

	UFUNCTION(Server, WithValidation, Reliable, BlueprintCallable, Category = "AONGameState")
	void ServerSetHeroAction(AHeroCharacter* hero, const FHeroAction& action);


	UFUNCTION(Server, WithValidation, Reliable, BlueprintCallable, Category = "AONGameState")
	void AppendHeroAction(AHeroCharacter* hero, const FHeroAction& action);

	UFUNCTION(Server, WithValidation, Reliable, BlueprintCallable, Category = "AONGameState")
	void ClearHeroAction(AHeroCharacter* hero, const FHeroAction& action);

	FVector2D GetMouseScreenPosition();

	void OnMouseRButtonPressed1();
	void OnMouseRButtonPressed2();
	void OnMouseRButtonReleased();

	void OnMouseLButtonPressed1();
	void OnMouseLButtonPressed2();
	void OnMouseLButtonReleased();

	
};


