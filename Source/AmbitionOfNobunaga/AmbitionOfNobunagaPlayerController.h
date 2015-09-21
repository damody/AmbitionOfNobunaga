// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/PlayerController.h"
#include "Engine/EngineBaseTypes.h"
#include "AmbitionOfNobunagaPlayerController.generated.h"

class ARTS_HUD;
class AHeroCharacter;
class AEquipment;

USTRUCT()
struct AMBITIONOFNOBUNAGA_API FHeroAttack
{
	GENERATED_USTRUCT_BODY()

	FHeroAttack(){}
	FHeroAttack(AHeroCharacter* heros, AHeroCharacter* dst) :
		Heros(heros), beAttack(dst){}
	AHeroCharacter* Heros;
	AHeroCharacter* beAttack;
};

USTRUCT()
struct AMBITIONOFNOBUNAGA_API FHeroThrow
{
	GENERATED_USTRUCT_BODY()

	FHeroThrow(){}
	FHeroThrow(AHeroCharacter* heros, const FVector& dst, int32 eid) :
		Heros(heros), DestLocation(dst), EquIndex(eid){}
	AHeroCharacter* Heros;
	FVector DestLocation;
	int32 EquIndex;
};

USTRUCT()
struct AMBITIONOFNOBUNAGA_API FHeroPickup
{
	GENERATED_USTRUCT_BODY()

	FHeroPickup(){}
	FHeroPickup(AHeroCharacter* heros, const FVector& dst, AEquipment* equ) :
		Heros(heros), DestLocation(dst), Equptr(equ){}
	AHeroCharacter* Heros;
	FVector DestLocation;
	AEquipment* Equptr;
};

USTRUCT()
struct AMBITIONOFNOBUNAGA_API FMoveData
{
	GENERATED_USTRUCT_BODY()

	FMoveData(){}
	FMoveData(const TArray<AHeroCharacter*>& heros, const FVector& dst) :Heros(heros), DestLocation(dst){}
	TArray<AHeroCharacter*> Heros;
	FVector DestLocation;
};

USTRUCT()
struct AMBITIONOFNOBUNAGA_API FMoveData2D
{
	GENERATED_USTRUCT_BODY()

	FMoveData2D(){}
	FMoveData2D(const TArray<AHeroCharacter*>& heros, const FVector2D& dst) :Heros(heros), DestLocation(dst){}
	TArray<AHeroCharacter*> Heros;
	FVector2D DestLocation;
};

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

	TArray<FMoveData> HeroMoveQueue;
	TArray<FMoveData2D> HeroMoveQueue2D;
	TArray<AHeroCharacter*> HeroStopMoveQueue;
	TArray<FHeroThrow> HeroThrowQueue;
	TArray<FHeroPickup> HeroPickupQueue;
	TArray<FHeroAttack> HeroAttackQueue;

	// Begin PlayerController interface
	virtual void PlayerTick(float DeltaTime) override;
	virtual void SetupInputComponent() override;
	// End PlayerController interface
	
	UFUNCTION(Server, WithValidation, Reliable)
	void ServerUpdateMove();

	UFUNCTION(Server, WithValidation, Reliable)
	void ServerMoveHeros(const FVector DestLocation, const TArray<AHeroCharacter*>& heros);

	UFUNCTION(Server, WithValidation, Reliable)
	void ServerStopMovement(const TArray<AHeroCharacter*>& heros);
public:
		
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RTS")
	FVector2D CurrentMouseXY;

	/** Navigate player to the given world location. */
	

	UFUNCTION(Server, WithValidation, Reliable, BlueprintCallable, Category = "RTS")
	void AddHeroToMoveQueue(const FVector DestLocation, const TArray<AHeroCharacter*>& heros);

	UFUNCTION(Server, WithValidation, Reliable, BlueprintCallable, Category = "RTS")
	void AddHeroToPickupQueue(const FVector DestLocation, AHeroCharacter* heros, AEquipment* equ);
		
	UFUNCTION(Server, WithValidation, Reliable, BlueprintCallable, Category = "RTS")
	void AddHeroToThrowQueue(const FVector DestLocation, AHeroCharacter* heros, int32 EquIndex);

	UFUNCTION(Server, WithValidation, Reliable, BlueprintCallable, Category = "RTS")
	void AddHeroToMoveQueue2D(const FVector2D DestLocation, const TArray<AHeroCharacter*>& heros);
	
	UFUNCTION(Server, WithValidation, Reliable, BlueprintCallable, Category = "RTS")
	void AddHeroToStopMoveQueue(AHeroCharacter* hero);

	UFUNCTION(Server, WithValidation, Reliable, BlueprintCallable, Category = "RTS")
	void AddHeroToAttackQueue(const TArray<AHeroCharacter*>& heros, AHeroCharacter* dst);

	FVector2D GetMouseScreenPosition();

	void OnMouseRButtonPressed();
	void OnMouseRButtonReleased();

	void OnMouseLButtonPressed();
	void OnMouseLButtonReleased();
};


