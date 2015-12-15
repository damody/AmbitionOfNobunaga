// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SkillHintActor.h"
#include "GameFramework/Character.h"
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
enum class EHeroStatusEnum : uint8
{
	Stand,
	Walk,
	AttackBegin,
	Attacking,
	AttackEnd,
};

class AEquipment;
class ABulletActor;

UCLASS()
class AMBITIONOFNOBUNAGA_API AHeroCharacter : public ACharacter
{
	GENERATED_UCLASS_BODY()

public:

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
	void BP_ImplementSkill(int32 index, FRotator RFaceTo, FVector VFaceTo, FVector Pos);
	
	UFUNCTION(BlueprintCallable, Category = "Hero")
    virtual bool UseSkill(int32 index, FRotator RFaceTo, FVector VFaceTo, FVector Pos);

	UFUNCTION(BlueprintCallable, Category = "Hero")
	int32 GetCurrentSkillIndex();

	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly)
	UDecalComponent* SelectionDecal;

	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly)
	UArrowComponent* PositionOnHead;

	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly)
	UArrowComponent* PositionUnderFoot;

	UPROPERTY(Category = Character, EditAnywhere, BlueprintReadWrite)
	TSubclassOf<ABulletActor> HeroBullet;

	UPROPERTY(Category = Character, EditAnywhere, BlueprintReadWrite, Replicated)
	EHeroStatusEnum HeroStatus;

	// 英雄名
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hero")
    FString HeroName;
	// 移動速度
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hero|Current")
	int32 TeamId;
	// 歷史說明
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hero")
    FString HeroHistoryDescription;

	// 攻擊動畫播到幾秒時發出攻擊
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hero")
	float AnimationInstantAttack;

	// 目前是否攻擊
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hero|Current")
	bool PlayAttack;
	// 當前普攻是否打出來了
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hero|Current")
	bool IsAttacked;
	// 目前攻擊動畫時間長度
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hero|Current")
	float CurrentAttackTime;
	// 目前攻擊計時器
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hero|Current")
	float CurrentAttackSpeedCount;
	// 目前等級
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hero|Current")
	int32 CurrentLevel;
	// 移動速度
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hero|Current")
	float CurrentMoveSpeed;
	// 血量
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hero|Current")
	float CurrentMaxHP;
	// 魔力
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hero|Current")
	float CurrentMaxMP;
	// 血量
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Hero|Current", Replicated)
	float CurrentHP;
	// 魔力
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Hero|Current")
	float CurrentMP;
	// 回血
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Hero|Current")
	float CurrentHealingHP;
	// 回魔
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Hero|Current")
	float CurrentHealingMP;
	// 攻速
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Hero|Current")
	float CurrentAttackSpeed;
	// 攻速秒數
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Hero|Current")
	float CurrentAttackSpeedSecond;
	// 攻擊力
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Hero|Current")
	float CurrentAttack;
	// 防禦力
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Hero|Current")
	float CurrentArmor;
	// 當前魔法減傷
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Hero|Current")
	float CurrentMagicInjured;
	// 外加力量
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Hero|Current")
	float AdditionStrength;
	// 外加敏捷
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Hero|Current")
	float AdditionAgility;
	// 外加智力
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Hero|Current")
	float AdditionIntelligence;
	// 目前攻擊距離
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Hero|Current")
	float CurrentAttackRadius;
	// 準備要用的技能索引
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Hero|Current")
	int32 CurrentSkillIndex;

	// 血條長度
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hero")
	float HPBarLength;

	// 裝備
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hero", Replicated)
	TArray<AEquipment*> Equipments;

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
	ASkillHintActor* CurrentSkillHint;

	// 儲存所有技能每個等級的CD時間
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Hero")
    TArray<FLevelCDs> Skill_LevelCDs;

	// 是否在CD中
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Hero")
    TArray<bool> Skill_CDing;
	
	// 當前CD秒數，CD秒數等於Skill_MaxCD時就是CD結束
    // Skill_CurrentCD will accumulation every frame
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Hero")
    TArray<float> Skill_CurrentCD;

	// 當前技能CD時間
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Hero")
    TArray<float> Skill_MaxCD;

	// 當前所有技能原始CD時間
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Hero")
    TArray<float> Skill_BaseCD;

	// 當前所有的技能等級
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Hero")
    TArray<int32> Skill_Level;

	// 可以使用的技能點數
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Hero")
    int32 Skill_Points;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Hero")
	bool isSelection;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hero")
	float PickupObjectDistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hero", Replicated)
	AEquipment* WantPickup;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hero", Replicated)
	AEquipment* WantThrow;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hero", Replicated)
	FVector ThrowDestination;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hero", Replicated)
	AHeroCharacter* WantAttack;
};
