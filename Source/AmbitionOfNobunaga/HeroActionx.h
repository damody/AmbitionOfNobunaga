// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include "HeroActionx.generated.h"


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
	MovingAttackActor,	// 邊移動邊攻擊指定敵人
	MoveAndAttack,		// A點地板 移動中遇到敵人停下來攻擊
	SpellToPosition,	// 指定地點的技能
	SpellToActor,		// 指定技
	SpellToDirection,	// 指向技
	SpellToSelf,		// 不需指定目標的技能
	MoveToPickup,		// 撿裝
	MoveToThrowEqu,		// 丟裝
	ThrowEquToActor		// 丟裝給人
};

/**
 *
 */
USTRUCT(BlueprintType)
struct FHeroAction
{
	GENERATED_USTRUCT_BODY()

	FHeroAction() :ActionStatus(EHeroActionStatus::Default), TargetHero(NULL), TargetEquipment(NULL),
		TargetValue(FVector::ZeroVector) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EHeroActionStatus ActionStatus;

	// for MoveToActor, FollowActor, AttackActor, MovingAttackActor, SpellToActor, ThrowEquToActor
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AHeroCharacter* TargetHero;

	// for MoveToPickup
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AEquipment* TargetEquipment;

	// for MoveToPosition, MoveAndAttack, SpellToPosition, SpellToDirection, MoveToThrowEqu
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector		TargetValue;

	bool operator==(const FHeroAction& rhs)
	{
		return rhs.ActionStatus == ActionStatus &&
			rhs.TargetHero == TargetHero &&
			rhs.TargetEquipment == TargetEquipment &&
			rhs.TargetValue == TargetValue;
	}
	bool operator!=(const FHeroAction& rhs)
	{
		return !(*this == rhs);
	}
};

UCLASS()
class UHeroActionx : public UObject
{
	GENERATED_BODY()

};
