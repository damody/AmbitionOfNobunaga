// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SkillHintActor.h"
#include "GameFramework/Character.h"
#include "AIController.h"
#include "Components/ArrowComponent.h"
#include "HeroActionx.h"
#include "HeroCharacter.generated.h"

USTRUCT(BlueprintType)
struct FLevelCDs
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<float> CDs;

	float operator[](int32 n)
	{
		return CDs[n];
	}
};

USTRUCT(BlueprintType)
struct FSkillDescription
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FString> DescriptionLevel;

	FString& operator[](int32 n)
	{
		return DescriptionLevel[n];
	}
};

UENUM(BlueprintType)
enum class EHeroBodyStatus : uint8
{
	Standing,
	Moving,
	Stunning,
	AttackWating, // 攻擊等待
	AttackBegining, // 攻擊前搖
	AttackEnding, //攻擊後搖
	SpellWating, // 施法前等待
	SpellBegining, // 施法前搖
	SpellEnding, // 施法後搖
};

class AEquipment;
class ABulletActor;

UCLASS()
class AMBITIONOFNOBUNAGA_API AHeroCharacter : public ACharacter
{
	GENERATED_UCLASS_BODY()

public:

	AAIController* WalkAI;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

	virtual void PostEditChangeChainProperty(FPropertyChangedChainEvent& PropertyChangedEvent) override;
#endif // WITH_EDITOR
	bool Pickup(AEquipment* equ);

	// for UI
	UFUNCTION()
	void OnMouseClicked(UPrimitiveComponent* TouchComp);

	UFUNCTION(BlueprintCallable, Category = "Hero")
	void SelectionOn();

	UFUNCTION(BlueprintCallable, Category = "Hero")
	void SelectionOff();

	void CheckSelf(bool res, FString msg);

	// for Game Logic
	UFUNCTION(BlueprintCallable, Category = "Hero")
	float GetSkillCDPercent(int32 n);

	UFUNCTION(BlueprintCallable, Category = "Hero")
	float GetHPPercent();

	UFUNCTION(BlueprintCallable, Category = "Hero")
	float GetMPPercent();

	// 依等級更新血魔攻速
	UFUNCTION(BlueprintCallable, Category = "Hero")
	void UpdateHPMPAS();

	// 依等級更新力敏智
	UFUNCTION(BlueprintCallable, Category = "Hero")
	void UpdateSAI();

	UFUNCTION(BlueprintCallable, Category = "Hero")
	bool ShowSkillHint(int32 index);

	UFUNCTION(BlueprintCallable, Category = "Hero")
	void HideSkillHint();

	UFUNCTION(BlueprintImplementableEvent)
	void BP_ImplementSkill(int32 index, FVector VFaceTo, FVector Pos);

	UFUNCTION(BlueprintCallable, Category = "Hero")
	bool UseSkill(int32 index, FVector VFaceTo, FVector Pos);

	UFUNCTION(BlueprintCallable, Category = "Hero")
	int32 GetCurrentSkillIndex();

	// 確定當前動作做完了沒
	bool CheckCurrentActionFinish();

	// 做動作
	void DoAction(const FHeroAction& CurrentAction);

	// 停止目前所有動作
	void DoNothing();

	// 做移動到指定位置
	void DoAction_MoveToPosition(const FHeroAction& CurrentAction);
	void DoAction_MoveToPositionImpl(const FHeroAction& CurrentAction);
	
	// 推出做完的動作
	void PopAction();
	// 做打人
	void DoAction_AttackActor(const FHeroAction& CurrentAction);

	// 做指向技
	void DoAction_SpellToDirection(const FHeroAction& CurrentAction);
	
	// 選人的地版光環
	UPROPERTY(Category = Character, EditAnywhere, BlueprintReadOnly)
	UDecalComponent * SelectionDecal;

	UPROPERTY(Category = Character, EditAnywhere, BlueprintReadOnly)
	UArrowComponent * PositionOnHead;

	UPROPERTY(Category = Character, EditAnywhere, BlueprintReadOnly)
	UArrowComponent * PositionUnderFoot;

	UPROPERTY(Category = Character, EditAnywhere, BlueprintReadWrite)
	TSubclassOf<ABulletActor> HeroBullet;

	// 英雄名
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hero")
	FString HeroName;

	// 歷史說明
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hero")
	FString HeroHistoryDescription;
	
	// 血條長度
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hero")
	float HPBarLength;

	// 大頭貼
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hero")
	UTexture2D * Head;

	// set by HUD
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hero")
	FVector2D	ScreenPosition;

	// 該英雄的技能數量
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hero")
	int32 Skill_Amount;

	// 目前攻擊距離
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hero")
	float BaseAttackRadius;
	// 攻速加乘
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hero")
	float AdditionAttackSpeed;
	// 基礎攻速
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hero")
	float BaseAttackSpeedSecond;

	// 基礎攻擊動畫時間長度
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hero")
	float BaseAttackingAnimationTimeLength;
	// 基礎攻擊前搖時間長度
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hero")
	float BaseAttackingBeginingTimeLength;
	// 基礎攻擊後搖時間長度
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hero")
	float BaseAttackingEndingTimeLength;

	// 基礎施法前等待時間長度
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hero")
	float BaseSpellingWatingTimeLength;
	// 基礎施法動畫時間長度
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hero")
	float BaseSpellingAnimationTimeLength;
	// 基礎施法前搖時間長度
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hero")
	float BaseSpellingBeginingTimeLength;
	// 基礎施法後搖時間長度
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hero")
	float BaseSpellingEndingTimeLength;

	
	// 追踨目標更新時間
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hero")
	float FollowActorUpdateTimeGap;


	// 基礎魔法受傷倍率
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hero")
	float BaseMagicInjuredRatio;
	// 基礎物理受傷倍率
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hero")
	float BasePhysicsInjuredRatio;
	// 基礎裝甲
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hero")
	float BaseArmor;
	// 基礎攻擊力
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hero")
	float BaseAttack;
	// 基礎移動速度
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hero")
	float BaseMoveSpeed;

	// 基礎回血
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hero")
	float BaseHealingHP;
	// 基礎回魔
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hero")
	float BaseHealingMP;
	// 基礎血量
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hero")
	float BaseHP;
	// 基礎魔力
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hero")
	float BaseMP;
	// 基礎力量
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hero")
	float BaseStrength;
	// 基礎敏捷
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hero")
	float BaseAgility;
	// 基礎智力
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hero")
	float BaseIntelligence;

	// 力量
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hero")
	float Strength;
	// 敏捷
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hero")
	float Agility;
	// 智力
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hero")
	float Intelligence;

	// 每個等級提升的XXX不累加
	// 每個等級提升的攻擊力
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Hero")
	TArray<float> LevelProperty_Attack;
	// 每個等級提升的力量
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Hero")
	TArray<float> LevelProperty_Strength;
	// 每個等級提升的敏捷
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Hero")
	TArray<float> LevelProperty_Agility;
	// 每個等級提升的智力
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Hero")
	TArray<float> LevelProperty_Intelligence;

	// 技能名字
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Hero")
	TArray<FString> Skill_Name;
	// 使用了技能後是否面對技能
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hero")
	TArray<bool> Skill_FaceSkill;
	// 技能描述
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Hero")
	TArray<FSkillDescription> Skill_Description;

	// 技能圖片
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Hero")
	TArray<UTexture2D*> Skill_Texture;

	// 所有技能提示
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Hero")
	TArray<TSubclassOf<ASkillHintActor>> Skill_HintActor;

	// 當前技能提示
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Hero")
	ASkillHintActor * CurrentSkillHint;

	// 儲存所有技能每個等級的CD時間
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Hero")
	TArray<FLevelCDs> Skill_LevelCDs;

	// 是否在CD中
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hero")
	TArray<bool> Skill_CDing;

	// 當前CD秒數，CD秒數等於Skill_MaxCD時就是CD結束
	// Skill_CurrentCD will accumulation every frame
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hero")
	TArray<float> Skill_CurrentCD;

	// 當前技能CD時間
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hero")
	TArray<float> Skill_MaxCD;

	// 當前所有技能原始CD時間
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hero")
	TArray<float> Skill_BaseCD;

	// 當前所有的技能等級
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hero")
	TArray<int32> Skill_Level;

	// 可以使用的技能點數
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hero")
	int32 Skill_Points;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hero")
	bool isSelection;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hero")
	float PickupObjectDistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hero")
	float MinimumDontMoveDistance;


	// 隊伍id
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Current")
	int32 TeamId;
	// 目前是否攻擊
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Current")
	bool PlayAttack;
	// 當前普攻是否打出來了
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Current")
	bool IsAttacked;

	// 目前是否倒下
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Current")
	bool IsDead;

	/*

	                             Each Attacking Time gap
	|---------------------------------------------------------------------------------------|
	                                                                 waiting for next attack
	                                                                |-----------------------|
	CurrentAttackingAnimationTimeLength
	|---------------------------------------------------------------|
	CurrentAttackingBeginingTimeLength
	|--------------------------------|
	  								 CurrentAttackingEndingTimeLength
									 |------------------------------|
									 ^
							    Cause Damage
	*/
	// 目前攻擊動畫時間長度
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Current")
	float CurrentAttackingAnimationTimeLength;
	// 目前攻擊前搖時間長度
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Current")
	float CurrentAttackingBeginingTimeLength;
	// 目前攻擊後搖時間長度
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Current")
	float CurrentAttackingEndingTimeLength;

	/*

    waiting for next attack
    |---------------|
	                CurrentSpellingAnimationTimeLength
	                |---------------------------------------------------------------|
	                CurrentSpellingBeginingTimeLength
	                |--------------------------------|
	  						                         CurrentSpellingEndingTimeLength
							                         |------------------------------|
							                         ^
                                               Spell cold down
	*/

	// 目前施法前等待時間長度
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hero")
	float CurrentSpellingWatingTimeLength;
	// 目前施法動畫時間長度
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hero")
	float CurrentSpellingAnimationTimeLength;
	// 目前施法前搖時間長度
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hero")
	float CurrentSpellingBeginingTimeLength;
	// 目前施法後搖時間長度
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hero")
	float CurrentSpellingEndingTimeLength;

	// 目前攻擊計時器
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Counting")
	float AttackingCounting;
	// 追踨目標計時器
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Counting")
	float FollowActorUpdateCounting;
	// 施法計時器
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Counting")
	float SpellingCounting;

	// 目前等級
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Current")
	int32 CurrentLevel;
	// 移動速度
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Current")
	float CurrentMoveSpeed;
	// 血量
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Current")
	float CurrentMaxHP;
	// 魔力
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Current")
	float CurrentMaxMP;
	// 血量
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Current", Replicated)
	float CurrentHP;
	// 魔力
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Current")
	float CurrentMP;
	// 回血
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Current")
	float CurrentHealingHP;
	// 回魔
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Current")
	float CurrentHealingMP;
	// 攻速
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Current")
	float CurrentAttackSpeed;
	// 攻速秒數
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Current")
	float CurrentAttackSpeedSecond;
	// 攻擊力
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Current")
	float CurrentAttack;
	// 防禦力
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Current")
	float CurrentArmor;
	// 當前魔法減傷
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Current")
	float CurrentMagicInjured;
	// 外加力量
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Current")
	float AdditionStrength;
	// 外加敏捷
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Current")
	float AdditionAgility;
	// 外加智力
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Current")
	float AdditionIntelligence;
	// 目前攻擊距離
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Current")
	float CurrentAttackRadius;
	// 準備要用的技能索引
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Current")
	int32 CurrentSkillIndex;
	// 裝備
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Current", Replicated)
	TArray<AEquipment*> Equipments;

	// 依序做完裡面的動作
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Current", Replicated)
	TArray<FHeroAction> ActionQueue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Current", Replicated)
	FHeroAction CurrentAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Current", Replicated)
	EHeroBodyStatus BodyStatus;

	FVector LastMoveTarget;
	FHeroAction LastUseSkill;
};
