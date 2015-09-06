// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

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

    virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

	virtual void PostEditChangeChainProperty(FPropertyChangedChainEvent& PropertyChangedEvent) override;

	void CheckSelf(bool res, FString msg);

    UFUNCTION(BlueprintCallable, Category = "Hero")
    float GetSkillCD(int32 n);

	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly)
	UDecalComponent* SelectionDecal;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hero")
    FString HeroName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hero")
    FString HeroHistoryDescription;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hero")
    UTexture2D * Head;

	// set by HUD
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hero")
	FVector2D	ScreenPosition;

	// 該英雄的技能數量
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hero")
    int32 Skill_Amount;

	//技能名字
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hero")
    TArray<FString> Skill_Name;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hero")
    TArray<FSkillDescription> Skill_Description;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hero")
    TArray<UTexture2D*> Skill_Texture;

	// 儲存所有技能每個等級的CD時間
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hero")
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
};
