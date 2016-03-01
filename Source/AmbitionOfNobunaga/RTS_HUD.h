// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/HUD.h"
#include "RTSHitBox.h"
#include "MouseEffect.h"
#include "RTS_HUD.generated.h"


UENUM(BlueprintType)
enum class ERTSStatusEnum : uint8
{
	Normal,
	Move,
	Attack,
	ThrowEquipment,
	SkillHint,
	ToNormal
};
UENUM(BlueprintType)
enum class ERTSClickEnum : uint8
{
	LastRightClick,
	LastLeftClick
};
class AAmbitionOfNobunagaPlayerController;
class AHeroCharacter;
class AEquipment;
class ASceneObject;
/**
 * 
 */
UCLASS()
class AMBITIONOFNOBUNAGA_API ARTS_HUD : public AHUD
{
	GENERATED_BODY()
public:
	ARTS_HUD();
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
    virtual void Tick(float DeltaSeconds) override;

	virtual void DrawHUD();

	bool CheckInSelectionBox(FVector2D pos);

	UFUNCTION(BlueprintCallable, Category = "RTS")
	void ClearAllSelection();

	TArray<FRTSHitBox>	RTS_HitBoxMap;

	FRTSHitBox* FindHitBoxByName(const FString& name);

	UFUNCTION(BlueprintImplementableEvent)
	void RTS_HitBoxRButtonPressed(const FString& name);

	UFUNCTION(BlueprintImplementableEvent)
	void RTS_HitBoxRButtonReleased(const FString& name);

	UFUNCTION(BlueprintImplementableEvent)
	void RTS_HitBoxLButtonPressed(const FString& name);

	UFUNCTION(BlueprintImplementableEvent)
	void RTS_HitBoxLButtonReleased(const FString& name);

	UFUNCTION(BlueprintImplementableEvent)
	void RTS_MouseRButtonPressed();

	UFUNCTION(BlueprintCallable, Category = "RTS")
	void RTS_AddHitBox(FVector2D Position, FVector2D Size, const FString& Name, bool bConsumesInput, int32 Priority);

	UFUNCTION(BlueprintCallable, Category = "RTS")
	bool IsGameRegion(FVector2D pos);

	UFUNCTION(BlueprintCallable, Category = "RTS")
	bool IsUIRegion(FVector2D pos);

	UFUNCTION(BlueprintCallable, Category = "RTS")
	void AssignSelectionHeroPickup(AEquipment* equ);
	
	UFUNCTION(BlueprintCallable, Category = "RTS")
	void HeroAttackHero(AHeroCharacter* hero);

	UFUNCTION(BlueprintCallable, Category = "RTS")
	void HeroAttackSceneObject(ASceneObject* SceneObj);
	
	// use skill callback by localcontroller
	void KeyboardCallUseSkill(int32 idx);
	
	UFUNCTION(BlueprintCallable, Category = "RTS")
	FVector GetCurrentDirection();

	UFUNCTION(BlueprintCallable, Category = "RTS")
	FRotator GetCurrentRotator();
		
	void OnSize();

	void OnMouseMove(FVector2D pos, FVector pos3d);
	void OnRMouseDown(FVector2D pos);
	void OnRMousePressed1(FVector2D pos);
	void OnRMousePressed2(FVector2D pos);
	void OnRMouseReleased(FVector2D pos);
	void OnLMouseDown(FVector2D pos);
	void OnLMousePressed1(FVector2D pos);
	void OnLMousePressed2(FVector2D pos);
	void OnLMouseReleased(FVector2D pos);	

	UFUNCTION(BlueprintImplementableEvent)
	void SelectedHero(AHeroCharacter* hero);

	UFUNCTION(BlueprintImplementableEvent)
	void UnSelectHero();
	
	UFUNCTION(BlueprintImplementableEvent)
	void GetEquipmentPosition(int32 n, FVector2D& pos, FVector2D& size);

	UFUNCTION(BlueprintImplementableEvent)
	void GetSkillPosition(int32 n, FVector2D& pos, FVector2D& size);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RTS")
	TSubclassOf<AMouseEffect> MouseEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RTS")
	TArray<AHeroCharacter*> HeroCanSelection;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RTS")
	TArray<AHeroCharacter*> CurrentSelection;

	TArray<AHeroCharacter*> RemoveSelection;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RTS")
	FVector2D InitialMouseXY;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RTS")
	FVector2D CurrentMouseXY;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RTS")
	FVector CurrentMouseHit;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RTS")
	FLinearColor	SelectionBoxLineColor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RTS")
	FLinearColor	SelectionBoxFillColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RTS")
	FLinearColor	HPBarForeColor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RTS")
	FLinearColor	HPBarBackColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RTS")
	ERTSClickEnum	ClickStatus;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RTS")
	float	HPBarHeight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RTS")
	FVector2D	HPBarOffset;

	bool ClickedSelected;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RTS")
	bool bClickHero;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RTS")
	bool bMouseRButton;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RTS")
	bool bMouseLButton;

	bool bNeedMouseRDown;
	bool bNeedMouseLDown;

	bool bLeftShiftDown;
	bool bRightShiftDown;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RTS")
	UTexture2D* NothingTexture;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RTS")
	UMaterialInterface* SkillMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RTS")
	TArray<UMaterialInstanceDynamic*> SkillDMaterials;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RTS")
	UMaterialInterface* EquipmentMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RTS")
	TArray<UMaterialInstanceDynamic*> EquipmentDMaterials;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RTS")
	UMaterialInterface* ThrowMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RTS")
	UMaterialInstanceDynamic* ThrowDMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RTS")
	ERTSStatusEnum RTSStatus;


	FString RButtonDownHitBox;
	FString RButtonUpHitBox;

	FString LButtonDownHitBox;
	FString LButtonUpHitBox;

	AAmbitionOfNobunagaPlayerController* LocalController;

	AEquipment* WantPickup;

	int32 EquipmentIndex;

	UTexture2D* ThrowTexture;

	float ViewportScale;

	int32 SequenceNumber;
};
