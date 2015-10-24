// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include "HeroBuff.generated.h"

/**
 * 
 */
UCLASS()
class AMBITIONOFNOBUNAGA_API UHeroBuff : public UObject
{
	GENERATED_BODY()
public:
	// Buff 優先權
	UPROPERTY(Category = "HeroBuff", EditAnywhere, BlueprintReadOnly)
	int32 Priority;
	// 是否禁止移動
	UPROPERTY(Category = "HeroBuff", EditAnywhere, BlueprintReadOnly)
	bool BanMove;
	// 是否禁止攻擊
	UPROPERTY(Category = "HeroBuff", EditAnywhere, BlueprintReadOnly)
	bool BanAttack;
	// 是否禁止使用技能
	UPROPERTY(Category = "HeroBuff", EditAnywhere, BlueprintReadOnly)
	bool BanSkill;
	// 是否禁止使用道具
	UPROPERTY(Category = "HeroBuff", EditAnywhere, BlueprintReadOnly)
	bool BanEquipment;
	// 護盾值
	UPROPERTY(Category = "HeroBuff", EditAnywhere, BlueprintReadOnly)
	bool ShieldValue;
	// 是否觸發傷害重新計算事件
	UPROPERTY(Category = "HeroBuff", EditAnywhere, BlueprintReadOnly)
	bool InjuredEvent;
	// 物理輸出傷害加成
	UPROPERTY(Category = "HeroBuff", EditAnywhere, BlueprintReadOnly)
	float PhysicalDamageOutputRatio;
	// 魔法輸出傷害加成
	UPROPERTY(Category = "HeroBuff", EditAnywhere, BlueprintReadOnly)
	float MagicDamageOutputRatio;
	// 能不能被法術指定
	UPROPERTY(Category = "HeroBuff", EditAnywhere, BlueprintReadOnly)
	bool CanBeSkillSight;
	// 能不能被普攻指定
	UPROPERTY(Category = "HeroBuff", EditAnywhere, BlueprintReadOnly)
	bool CanBeAttackSight;

	// 是否能疊加
	UPROPERTY(Category = "HeroBuff", EditAnywhere, BlueprintReadOnly)
	bool CanSuperposition;
	// 疊加層數
	UPROPERTY(Category = "HeroBuff", EditAnywhere, BlueprintReadOnly)
	int32 Superposition;
	// 每一層持續時間
	UPROPERTY(Category = "HeroBuff", EditAnywhere, BlueprintReadOnly)
	float Duration;

	// 移動速度
	UPROPERTY(Category = "HeroBuff", EditAnywhere, BlueprintReadOnly)
	float MoveRatio;
	
};
