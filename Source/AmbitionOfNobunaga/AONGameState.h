// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameState.h"
#include "AONGameState.generated.h"


class AActor;
/**
 * 
 */
UCLASS()
class AMBITIONOFNOBUNAGA_API AAONGameState : public AGameState
{
	GENERATED_BODY()
public:

	UFUNCTION(Server, WithValidation, Reliable, BlueprintCallable, Category = "AONGameState")
	void SetObjectLocation(AActor* actor, FVector pos);
	
	// 每1點力量增加血量
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AONGameState")
	float StrengthToHP;
	// 每1點力量增加回血/sec
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AONGameState")
	float StrengthToHealingHP;

	// 每1點智力增加魔力
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AONGameState")
	float IntelligenceToMP;
	// 每1點智力增加回魔/sec
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AONGameState")
	float IntelligenceToHealingMP;

	// 每1點敏捷增加防禦
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AONGameState")
	float AgilityToDefense;
	// 每1點敏捷增加攻速
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AONGameState")
	float AgilityToAttackSpeed;
	// 每1點敏捷增加跑速
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AONGameState")
	float AgilityToWalkSpeed;

	UFUNCTION(BlueprintCallable, Category = "AONGameState")
	float ArmorConvertToInjuryPersent(float armor);
	
};
