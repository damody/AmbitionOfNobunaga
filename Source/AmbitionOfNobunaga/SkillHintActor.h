// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/SceneComponent.h"
#include "PaperSpriteComponent.h"
#include "GameFramework/Actor.h"
#include "SkillHintActor.generated.h"


UENUM(BlueprintType)
enum class ESkillHintEnum : uint8
{
	NoneHint,
	DirectionSkill_CanBlock,
	DirectionSkill,
	RangeSkill,
	EarmarkHeroSkill,
	EarmarkNonHeroSkill,
	EarmarkAnyoneSkill,
};

UCLASS()
class AMBITIONOFNOBUNAGA_API ASkillHintActor : public AActor
{
	GENERATED_UCLASS_BODY()
public:	

	// Component
	UPROPERTY(Category = "SkillHint", EditAnywhere, BlueprintReadOnly)
	FVector2D MouseIconOffset;

	UPROPERTY(Category = "FlySkill", VisibleAnywhere, BlueprintReadOnly)
	USceneComponent* Scene;

	UPROPERTY(Category = "FlySkill", VisibleAnywhere, BlueprintReadOnly)
	UPaperSpriteComponent* BodySprite;

	UPROPERTY(Category = "FlySkill", VisibleAnywhere, BlueprintReadOnly)
	UPaperSpriteComponent* HeadSprite;

	UPROPERTY(Category = "FlySkill", VisibleAnywhere, BlueprintReadOnly)
	UPaperSpriteComponent* FootSprite;

	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

	virtual void PostInitProperties() override;

	UFUNCTION(Category = "FlySkill", BlueprintCallable)
	void SetLength(float len);

	void UpdateLength();

	void UpdatePos(FVector PlayerPos, FVector MousePos);

	UPROPERTY(Category = "SkillHint", EditAnywhere, BlueprintReadOnly)
	ESkillHintEnum SkillType;

	UPROPERTY(Category = "SkillHint", EditAnywhere, BlueprintReadOnly)
	FVector SkillPos;

	UPROPERTY(Category = "SkillHint", VisibleAnywhere, BlueprintReadOnly)
	bool UseDirectionSkill;

	UPROPERTY(Category = "SkillHint", VisibleAnywhere, BlueprintReadOnly)
	bool UseRangeSkill;
	
	UPROPERTY(Category = "SkillHint", EditAnywhere, BlueprintReadOnly)
	UTexture2D* MouseIcon;
	
	// 是否固定長度
	UPROPERTY(Category = "FlySkill", EditAnywhere, BlueprintReadOnly, meta = (EditCondition = "UseDirectionSkill"))
	bool IsFixdLength;

	// 技能最遠距離
	UPROPERTY(Category = "FlySkill", EditAnywhere, BlueprintReadOnly, meta = (EditCondition = "UseDirectionSkill"))
	float SkillLength;

	// 技能最短距離
	UPROPERTY(Category = "FlySkill", EditAnywhere, BlueprintReadOnly, meta = (EditCondition = "UseDirectionSkill"))
	float MinimalLength;

	// 法術直徑
	UPROPERTY(Category = "FlySkill", EditAnywhere, BlueprintReadOnly, meta = (EditCondition = "UseRangeSkill"))
	float SkillDiameter;

	// 施法距離
	UPROPERTY(Category = "FlySkill", EditAnywhere, BlueprintReadOnly, meta = (EditCondition = "UseRangeSkill"))
	float SkillCastRadius;

	// 技能最小施法距離
	UPROPERTY(Category = "FlySkill", EditAnywhere, BlueprintReadOnly, meta = (EditCondition = "UseRangeSkill"))
	float MinimalCastRadius;
};
