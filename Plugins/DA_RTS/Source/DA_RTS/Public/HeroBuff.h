// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include "HeroBuff.generated.h"

UENUM(BlueprintType)
enum class EHeroBuffKind : uint8
{
	Default,	// 無特殊效果
	Dazzing,	// 暈炫
	BanMove,	// 禁移動
	BanAttack,	// 禁攻擊
	BanSkill,	// 禁技能
	BanEquipment,	// 禁道具
	HasShield,		// 有護盾
	HasPhysicalDamageOutputRatio,	// 物理傷害加成
	HasMagicDamageOutputRatio,		// 魔法傷害加成
	HasMoveRatio,	// 移動速度加成
	HasInjuredEvent,	// 觸發傷害重新計算事件
	BanBeSkillSight,	// 不能被法術指定
	BanBeAttackSight	// 不能被普攻指定

};

/**
 * 
 */

UCLASS(BlueprintType)
class DA_RTS_API UHeroBuff : public UObject
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category = "HeroBuff")
	static UHeroBuff* NewHeroBuff();

	// Buff 優先權
	UPROPERTY(Category = "HeroBuff", EditAnywhere, BlueprintReadOnly)
	int32 Priority;
	// Buff 名字
	UPROPERTY(Category = "HeroBuff", EditAnywhere, BlueprintReadOnly)
	FString Name;
	// 大頭貼
	UPROPERTY(Category = "HeroBuff", EditAnywhere, BlueprintReadOnly)
	UTexture2D * Head;
	// 是否暈炫
	UPROPERTY(Category = "HeroBuff", EditAnywhere, BlueprintReadOnly)
	bool Dazzing;
	
	UPROPERTY(Category = "HeroBuff", EditAnywhere, BlueprintReadOnly)
	EHeroBuffKind BuffKind;

	// 是否能疊加
	UPROPERTY(Category = "HeroBuff", EditAnywhere, BlueprintReadOnly)
	bool CanSuperposition;
	// 疊加層數
	UPROPERTY(Category = "HeroBuff", EditAnywhere, BlueprintReadOnly)
	int32 Superposition;
	// 每一層持續時間
	UPROPERTY(Category = "HeroBuff", EditAnywhere, BlueprintReadOnly)
	float Duration;
};

