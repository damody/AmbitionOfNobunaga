// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "HeroCharacter.generated.h"

UCLASS()
class AMBITIONOFNOBUNAGA_API AHeroCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AHeroCharacter();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

	UFUNCTION(BlueprintCallable, Category = "Hero")
	float GetSkillCD1();
	UFUNCTION(BlueprintCallable, Category = "Hero")
	float GetSkillCD2();
	UFUNCTION(BlueprintCallable, Category = "Hero")
	float GetSkillCD3();
	UFUNCTION(BlueprintCallable, Category = "Hero")
	float GetSkillCD4();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hero")
	UTexture2D* Head;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hero")
	UTexture2D* Tex_Skill1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hero")
	UTexture2D* Tex_Skill2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hero")
	UTexture2D* Tex_Skill3;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hero")
	UTexture2D* Tex_Skill4;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hero")
	TArray<float> Skill_LevelCDs1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hero")
	TArray<float> Skill_LevelCDs2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hero")
	TArray<float> Skill_LevelCDs3;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hero")
	TArray<float> Skill_LevelCDs4;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hero")
	bool Skill_CDing_1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hero")
	bool Skill_CDing_2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hero")
	bool Skill_CDing_3;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hero")
	bool Skill_CDing_4;

	// Skill_CurrentCD will accumulation every frame

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Hero")
	float Skill_CurrentCD_1;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Hero")
	float Skill_CurrentCD_2;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Hero")
	float Skill_CurrentCD_3;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Hero")
	float Skill_CurrentCD_4;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Hero")
	float Skill_MaxCD_1;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Hero")
	float Skill_MaxCD_2;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Hero")
	float Skill_MaxCD_3;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Hero")
	float Skill_MaxCD_4;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Hero")
	float Skill_BaseCD_1;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Hero")
	float Skill_BaseCD_2;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Hero")
	float Skill_BaseCD_3;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Hero")
	float Skill_BaseCD_4;
};
