// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameState.h"
#include "HeroActionx.h"
#include "AONGameState.generated.h"

/**
 * 有需要全地圖大招可以改這裡的參數
 * if any hero need big spell, you can modify this parameter
 */
UCLASS()
class AMBITIONOFNOBUNAGA_API AAONGameState : public AGameState
{
	GENERATED_BODY()
public:

	UFUNCTION(NetMulticast, WithValidation, Reliable, BlueprintCallable, Category = "AONGameState")
	void SetObjectLocation(AActor* actor, const FVector& pos);

	UFUNCTION(NetMulticast, WithValidation, Reliable, BlueprintCallable, Category = "AONGameState")
	void CharacterMove(AHeroCharacter* hero, const FVector& action);

	UFUNCTION(NetMulticast, WithValidation, Reliable, BlueprintCallable, Category = "AONGameState")
	void CharacterStopMove(AHeroCharacter* hero);

	UFUNCTION(NetMulticast, WithValidation, Reliable, BlueprintCallable, Category = "AONGameState")
	void HeroUseSkill(AHeroCharacter* hero, int32 index, const FVector& VFaceTo, const FVector& Pos);

	UFUNCTION(NetMulticast, WithValidation, Reliable, BlueprintCallable, Category = "AONGameState")
	void SetHeroAction(AHeroCharacter* hero, const FHeroAction& action);

	UFUNCTION(NetMulticast, WithValidation, Reliable, BlueprintCallable, Category = "AONGameState")
	void AppendHeroAction(AHeroCharacter* hero, const FHeroAction& action);

	UFUNCTION(Server, WithValidation, Reliable, BlueprintCallable, Category = "AONGameState")
	void ClearHeroAction(AHeroCharacter* hero, const FHeroAction& action);
	
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
