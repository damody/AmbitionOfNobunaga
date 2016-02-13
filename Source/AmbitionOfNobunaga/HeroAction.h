// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include "HeroAction.generated.h"

class AHeroCharacter;
class AEquipment;

UENUM(BlueprintType)
enum class EHeroActionStatus : uint8
{
	Default,			// 剛被初始化
	MoveToPosition,		// 移動到指定目標
	MoveToActor,		// 移動到指定的人身邊
	FollowActor,		// 跟著人走
	AttackActor,		// 打指定的人
	MovingAttack,		// 邊移動邊攻擊
	MoveAndAttack,		// A點地板 移動中遇到敵人停下來攻擊
	SpellToPosition,	// 指定地點的技能
	SpellToActor,		// 指定技
	SpellToDirection,	// 指向技
	SpellToSelf,		// 不需指定目標的技能
	MoveToPickup,		// 撿裝
	MoveToThrowEqu		// 丟裝
};

/**
 * 
 */
UCLASS()
class AMBITIONOFNOBUNAGA_API UHeroAction : public UObject
{
	GENERATED_BODY()
public:
	EHeroActionStatus ActionStatus;

	AHeroCharacter* TargetHero;
	AEquipment* TargetEquipment;
	FVector		TargetPosition;

};
