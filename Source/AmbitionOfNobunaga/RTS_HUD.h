// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/HUD.h"
#include "RTS_HUD.generated.h"

UENUM(BlueprintType)
enum class ERTSStatusEnum : uint8
{
	Normal,
	Move,
	Attack
};

class AHeroCharacter;
/**
 * 
 */
UCLASS()
class AMBITIONOFNOBUNAGA_API ARTS_HUD : public AHUD
{
	GENERATED_BODY()
public:
	ARTS_HUD();
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
    virtual void Tick(float DeltaSeconds) override;

	virtual void DrawHUD();

	bool CheckInSelectionBox(FVector2D pos);

	UFUNCTION(BlueprintCallable, Category = "RTS")
	void ClearAllSelection();

	void OnMouseMove(FVector2D pos);
	void OnRMousePressed(FVector2D pos);
	void OnRMouseReleased(FVector2D pos);
	void OnLMousePressed(FVector2D pos);
	void OnLMouseReleased(FVector2D pos);
		
	UFUNCTION(BlueprintImplementableEvent)
	void SelectedHero(AHeroCharacter* hero);

	UFUNCTION(BlueprintImplementableEvent)
	void UnSelectHero();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RTS")
	TArray<AHeroCharacter*> HeroCanSelection;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RTS")
	TArray<AHeroCharacter*> CurrentSelection;

	TArray<AHeroCharacter*> RemoveSelection;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RTS")
	FVector2D InitialMouseXY;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RTS")
	FVector2D CurrentMouseXY;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RTS")
	FLinearColor	SelectionBoxLineColor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RTS")
	FLinearColor	SelectionBoxFillColor;

	bool ClickedSelected;

protected:
	uint32 bMouseRButton : 1;
	uint32 bMouseLButton : 1;

};
