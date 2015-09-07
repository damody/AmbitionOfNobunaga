// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/HUD.h"
#include "RTS_HUD.generated.h"


class AHeroCharacter;
/**
 * 
 */
UCLASS()
class AMBITIONOFNOBUNAGA_API ARTS_HUD : public AHUD
{
	GENERATED_BODY()
public:
	
	// Called every frame
    virtual void Tick(float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RTS")
	TArray<AHeroCharacter*> HeroCanSelection;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RTS")
	TArray<AHeroCharacter*> CurrentSelection;

	UFUNCTION(BlueprintCallable, Category = "RTS")
	void ClearAllSelection();
};
