// Fill out your copyright notice in the Description page of Project Settings.

#include "AmbitionOfNobunaga.h"
#include "GameFramework/Character.h"
// for GEngine
#include "Engine.h"

#include "AONGameState.h"
#include "RTS_HUD.h"
#include "HeroCharacter.h"
#include "Equipment.h"
#include "UnrealNetwork.h"
#include "BulletActor.h"
#include "MouseEffect.h"
#include "AmbitionOfNobunagaPlayerController.h"

AHeroCharacter::AHeroCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(FObjectInitializer::Get())
{
	HeroBullet = NULL;
	bReplicates = true;
	PrimaryActorTick.bCanEverTick = true;
	SelectionDecal = ObjectInitializer.CreateDefaultSubobject<UDecalComponent>(this, TEXT("SelectionDecal0"));
	PositionOnHead = ObjectInitializer.CreateDefaultSubobject<UArrowComponent>(this, TEXT("PositionOnHead0"));
	PositionUnderFoot = ObjectInitializer.CreateDefaultSubobject<UArrowComponent>(this, TEXT("PositionUnderFoot0"));

	PositionOnHead->AttachParent = GetCapsuleComponent();
	PositionUnderFoot->AttachParent = GetCapsuleComponent();

	SelectionDecal->SetWorldLocation(FVector(0, 0, -90));
	// FRotator = rotation Y Z X
	SelectionDecal->SetWorldRotation(FQuat(FRotator(90, 0, 0)));
	SelectionDecal->SetWorldScale3D(FVector(10, 50, 50));
	SelectionDecal->AttachParent = GetCapsuleComponent();

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	BodyStatus = EHeroBodyStatus::Standing;
	GetMesh()->SetWorldRotation(FQuat(FRotator(0, -90, 0)));

	// 目前攻擊動畫時間長度
	CurrentAttackingAnimationTimeLength = 0.5;
	// 基礎血量
	BaseHP = 450;
	// 基礎魔力
	BaseMP = 100;
	// 基礎撿物品距離
	PickupObjectDistance = 500;
	// 基礎等級
	CurrentLevel = 1;
	// 基礎攻速
	BaseAttackSpeedSecond = 1.8;
	IsAttacked = false;
	IsDead = false;
	LastMoveTarget = FVector::ZeroVector;
	AttackingCounting = 0;
	FollowActorUpdateCounting = 0;
	SpellingCounting = 0;

	// 基礎攻擊前搖時間長度
	BaseAttackingBeginingTimeLength = 0.5;
	// 基礎攻擊後搖時間長度
	BaseAttackingEndingTimeLength = 0.3;
	// 基礎攻擊動畫時間長度
	BaseAttackingAnimationTimeLength = 0.5;

	// 基礎施法前等待時間長度
	BaseSpellingWatingTimeLength = 0;
	// 基礎施法動畫時間長度
	BaseSpellingAnimationTimeLength = 1;
	// 基礎施法前搖時間長度
	BaseSpellingBeginingTimeLength = 0.5;
	// 基礎施法後搖時間長度
	BaseSpellingEndingTimeLength = 0.5;


	// 每隔一段時間更新移動
	FollowActorUpdateTimeGap = 0.3;
	GetCapsuleComponent()->OnClicked.AddDynamic(this, &AHeroCharacter::OnMouseClicked);
}

// Called when the game starts or when spawned
void AHeroCharacter::BeginPlay()
{
	Super::BeginPlay();
	WalkAI = nullptr;
	if (Role == ROLE_Authority)
	{
		// Get current location of the Player Proxy
		FVector Location = GetActorLocation();
		FRotator Rotation = GetActorRotation();

		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = Instigator;
		SpawnParams.bNoCollisionFail = true;

		WalkAI = GetWorld()->SpawnActor<AAIController>(GetActorLocation(), GetActorRotation());
		WalkAI->Possess(this);
	}

	SelectionDecal->SetVisibility(false);
	isSelection = false;
	CheckSelf(Skill_MaxCD.Num() == Skill_Amount, TEXT("Skill_MaxCD is invalid"));
	CheckSelf(Skill_Description.Num() == Skill_Amount, TEXT("Skill_Description is invalid"));
	CheckSelf(Skill_Texture.Num() == Skill_Amount, TEXT("Skill_Texture is invalid"));
	CheckSelf(Skill_LevelCDs.Num() == Skill_Amount, TEXT("Skill_LevelCDs is invalid"));
	CheckSelf(Skill_CDing.Num() == Skill_Amount, TEXT("Skill_CDing is invalid"));
	CheckSelf(Skill_CurrentCD.Num() == Skill_Amount, TEXT("Skill_CurrentCD is invalid"));
	CheckSelf(Skill_BaseCD.Num() == Skill_Amount, TEXT("Skill_BaseCD is invalid"));
	CheckSelf(Skill_Level.Num() == Skill_Amount, TEXT("Skill_Level is invalid"));

	for(int32 i = 0; i < Skill_MaxCD.Num(); ++i)
	{
		if(Skill_MaxCD.Num() > 0)
		{
			Skill_MaxCD[i] = Skill_BaseCD[i];
			Skill_CurrentCD[i] = Skill_BaseCD[i];
		}
	}

	Equipments.SetNum(6);
	for(int32 idx = 0; idx < Equipments.Num(); ++idx)
	{
		Equipments[idx] = NULL;
	}
	CurrentSkillHint = NULL;
	CurrentSkillIndex = -1;
	// 依等級更新力敏智
	UpdateSAI();
	// 依等級更新血魔攻速
	UpdateHPMPAS();
	CurrentHP = CurrentMaxHP;
	CurrentMP = CurrentMaxMP;
	CurrentAttackRadius = BaseAttackRadius;
	CurrentAttack = BaseAttack;
	CurrentArmor = BaseArmor;
	CurrentAttackingBeginingTimeLength = BaseAttackingBeginingTimeLength;
	CurrentAttackingEndingTimeLength = BaseAttackingEndingTimeLength;
	CurrentAttackingAnimationTimeLength = BaseAttackingAnimationTimeLength;
	CurrentSpellingWatingTimeLength = BaseSpellingWatingTimeLength;
	CurrentSpellingAnimationTimeLength = BaseSpellingAnimationTimeLength;
	CurrentSpellingBeginingTimeLength = BaseSpellingBeginingTimeLength;
	CurrentSpellingEndingTimeLength = BaseSpellingEndingTimeLength;

	MinimumDontMoveDistance = GetCapsuleComponent()->GetScaledCapsuleHalfHeight() + 30;
}

// Called every frame
void AHeroCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (CurrentSkillHint)
	{
		ARTS_HUD* hud = Cast<ARTS_HUD>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());
		if (hud)
		{
			CurrentSkillHint->UpdatePos(GetActorLocation(), hud->CurrentMouseHit);
		}
	}
	if(CurrentHP <= 0)
	{
		IsDead = true;
		CurrentHP = 0;
	}
	else
	{
		IsDead = false;
	}
	AttackingCounting += DeltaTime;
	FollowActorUpdateCounting += DeltaTime;
	SpellingCounting += DeltaTime;
	// 算CD
	for(int32 i = 0; i < Skill_CDing.Num(); ++i)
	{
		if(Skill_CDing[i])
		{
			Skill_CurrentCD[i] += DeltaTime;
			if(Skill_CurrentCD[i] > Skill_MaxCD[i])
			{
				Skill_CurrentCD[i] = Skill_MaxCD[i];
				Skill_CDing[i] = false;
			}
		}
	}
	// 是否有動作？
	if (ActionQueue.Num() > 0)
	{
		// 動作駐列最上層動作是否為當前動作
		if (ActionQueue[0] != CurrentAction)
		{
			// 拿出動作
			CurrentAction = ActionQueue[0];
			// 進入此狀態的有限狀態機來做事
			DoAction(CurrentAction);
		}
		// 查看當前動作是否做完？
		else if (!CheckCurrentActionFinish())
		{
			// 進入此狀態的有限狀態機來做事
			DoAction(CurrentAction);
		}
		else
		{
			// 推出事件
			PopAction();
			// 檢查動作駐列是否為空？
			if (ActionQueue.Num() == 0)
			{
				// 站立不動
				DoNothing();
			}
			else
			{
				// 進入此狀態的有限狀態機來做事
				DoAction(CurrentAction);
			}
		}
	}
	else
	{
		// 站立不動
		DoNothing();
	}


	/*
	// 撿物品
	if(WantPickup)
	{
	    // 判斷距離
	    if(FVector::Dist(GetActorLocation(), WantPickup->GetActorLocation()) < PickupObjectDistance)
	    {
	        if(Pickup(WantPickup))
	        {
	            WantPickup->ServerSetLocation(FVector(FMath::Rand(), 99999, 99999));
	        }
	        WantPickup = NULL;
	        // 停止移動
	        ARTS_HUD* hud = Cast<ARTS_HUD>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());
	        if(hud)
	        {
	            hud->StopMovementHero(this);
	        }
	        HeroStatus = EHeroBodyStatus::Standing;
	    }
	}
	// 丟物品
	else if(WantThrow)
	{
	    if(FVector::Dist(GetActorLocation(), ThrowDestination) < PickupObjectDistance)
	    {
	        FVector origin, extent;
	        WantThrow->GetActorBounds(true, origin, extent);
	        ThrowDestination.Z += extent.Z;
	        for(int32 idx = 0; idx < Equipments.Num(); ++idx)
	        {
	            if(Equipments[idx] == WantThrow)
	            {
	                if(idx == 0)
	                {
	                    Equipments[0]->DetachRootComponentFromParent();
	                }
	                Equipments[idx] = NULL;
	            }
	        }
	        WantThrow->ServerSetLocation(ThrowDestination);
	        WantThrow = NULL;
	        // 停止移動
	        ARTS_HUD* hud = Cast<ARTS_HUD>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());
	        if(hud)
	        {
	            hud->StopMovementHero(this);
	        }
	        HeroStatus = EHeroBodyStatus::Standing;
	    }
	}
	// 打人啦~
	else if(WantAttack)
	{
	    FVector dir = WantAttack->GetActorLocation() - GetActorLocation();
	    SetActorRotation(dir.Rotation());
	    switch(HeroStatus)
	    {
	    case EHeroBodyStatus::Standing:
	    {
	        if(FVector::Dist(GetActorLocation(), WantAttack->GetActorLocation()) > CurrentAttackRadius)
	        {
	            ARTS_HUD* hud = Cast<ARTS_HUD>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());
	            if(hud)
	            {
	                hud->HeroMove(this, WantAttack->GetActorLocation());
	            }
	        }
	        else
	        {
	            HeroStatus = EHeroBodyStatus::AttackWating;
	            CurrentAttackSpeedCount = 0;
	            IsAttacked = false;
	            PlayAttack = true;
	        }
	    }
	    break;
	    case EHeroBodyStatus::Moving:
	    {
	        if(FVector::Dist(GetActorLocation(), WantAttack->GetActorLocation()) < CurrentAttackRadius)
	        {
	            GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, HeroName + TEXT(" Hit!"));
	            ARTS_HUD* hud = Cast<ARTS_HUD>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());
	            if(hud)
	            {
	                hud->StopMovementHero(this);
	            }
	            HeroStatus = EHeroBodyStatus::AttackWating;
	            CurrentAttackSpeedCount = 0;
	            IsAttacked = false;
	            PlayAttack = true;
	        }
	    }
	    break;
	    case EHeroBodyStatus::AttackWating:
	    {
	        // 時間到就攻擊
	        if(!IsAttacked && CurrentAttackSpeedCount > AnimationInstantAttack)
	        {
	            IsAttacked = true;
	            AAONGameState* ags = Cast<AAONGameState>(UGameplayStatics::GetGameState(GetWorld()));
	            float Injury = ags->ArmorConvertToInjuryPersent(WantAttack->CurrentArmor);
	            float Damage = this->CurrentAttack * Injury;

	            if(HeroBullet)
	            {
	                FVector pos = GetActorLocation();
	                ABulletActor* bullet = GetWorld()->SpawnActor<ABulletActor>(HeroBullet);
	                if(bullet)
	                {
	                    bullet->SetActorLocation(pos);
	                    bullet->SetTartgetActor(WantAttack);
	                    bullet->Damage = Damage;
	                }
	            }
	            else
	            {
	                WantAttack->CurrentHP -= Damage;
	            }
	        }
	        if(CurrentAttackSpeedCount > CurrentAttackSpeedSecond)
	        {
	            CurrentAttackSpeedCount = 0;
	            HeroStatus = EHeroBodyStatus::AttackBegining;
	        }
	        // 播放攻擊動畫
	        // ...

	    }
	    break;
	    case EHeroBodyStatus::AttackBegining:
	    {
	        if(CurrentAttackSpeedCount > CurrentAttackTime)
	        {
	            HeroStatus = EHeroBodyStatus::AttackEnding;
	        }
	    }
	    break;
	    case EHeroBodyStatus::AttackEnding:
	    {
	        // 如果播完了攻擊
	        HeroStatus = EHeroBodyStatus::Standing;
	    }
	    break;
	    }
	}
	*/
}

// Called to bind functionality to input
void AHeroCharacter::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

}
#if WITH_EDITOR

void AHeroCharacter::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	//Get the name of the property that was changed
	FName PropertyName = (PropertyChangedEvent.Property != nullptr) ? PropertyChangedEvent.Property->GetFName() : NAME_None;

	if((PropertyName == GET_MEMBER_NAME_CHECKED(AHeroCharacter, Skill_LevelCDs)))
	{
		Skill_BaseCD.SetNum(Skill_LevelCDs.Num());
		Skill_MaxCD.SetNum(Skill_LevelCDs.Num());
		Skill_CDing.SetNum(Skill_LevelCDs.Num());
		Skill_CurrentCD.SetNum(Skill_LevelCDs.Num());
		Skill_Level.SetNum(Skill_LevelCDs.Num());
		Skill_FaceSkill.SetNum(Skill_LevelCDs.Num());
		for(int32 i = 0; i < Skill_LevelCDs.Num(); ++i)
		{
			if(Skill_LevelCDs[i].CDs.Num() > 0)
			{
				Skill_BaseCD[i] = Skill_LevelCDs[i][0];
				Skill_MaxCD[i] = Skill_BaseCD[i];
			}
		}
	}
	Super::PostEditChangeProperty(PropertyChangedEvent);
}

void AHeroCharacter::PostEditChangeChainProperty(FPropertyChangedChainEvent& PropertyChangedEvent)
{
	const FName TailPropName = PropertyChangedEvent.PropertyChain.GetTail()->GetValue()->GetFName();
	static FName Mobility_NAME(TEXT("CDs"));
	if(TailPropName == Mobility_NAME)
	{
		for(int32 i = 0; i < Skill_LevelCDs.Num(); ++i)
		{
			if(Skill_LevelCDs[i].CDs.Num() > 0)
			{
				Skill_BaseCD[i] = Skill_LevelCDs[i][0];
				Skill_MaxCD[i] = Skill_BaseCD[i];
			}
		}
	}
	Super::PostEditChangeChainProperty(PropertyChangedEvent);
}
#endif // WITH_EDITOR

bool AHeroCharacter::Pickup(AEquipment* equ)
{
	for(int32 idx = 0; idx < Equipments.Num(); ++idx)
	{
		if(Equipments[idx] == equ)
		{
			return false;
		}
	}
	for(int32 idx = 0; idx < Equipments.Num(); ++idx)
	{
		if(Equipments[idx] == NULL)
		{
			Equipments[idx] = equ;
			if(idx == 0)
			{
				Equipments[0]->AttachRootComponentTo(GetMesh(), TEXT("hand_rSocket"), EAttachLocation::SnapToTarget);
				return false;
			}
			return true;
		}
	}
	return false;
}

void AHeroCharacter::OnMouseClicked(UPrimitiveComponent* TouchComp)
{
	ARTS_HUD* hud = Cast<ARTS_HUD>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());
	if(hud)
	{
		// 按下左鍵
		if(hud->bMouseLButton)
		{
			if(hud->CurrentSelection.Num() == 1)
			{
				if(hud->CurrentSelection[0] == this)
				{
					return;
				}
				if (hud->CurrentSelection[0]->CurrentSkillHint)
				{
					return;
				}
			}
			hud->ClickedSelected = true;
			hud->ClearAllSelection();
			SelectionOn();
		}
		// 按下右鍵
		else if(hud->bMouseRButton)
		{
			if(hud->CurrentSelection.Num() > 0)
			{
				hud->HeroAttack(this);
			}
		}
	}
}

void AHeroCharacter::SelectionOn()
{
	isSelection = true;
	SelectionDecal->SetVisibility(true);
	ARTS_HUD* hud = Cast<ARTS_HUD>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());
	if(hud)
	{
		hud->CurrentSelection.Add(this);
	}
}

void AHeroCharacter::SelectionOff()
{
	isSelection = false;
	SelectionDecal->SetVisibility(false);
	ARTS_HUD* hud = Cast<ARTS_HUD>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());
	if(hud)
	{
		hud->RemoveSelection.Add(this);
	}
	HideSkillHint();
}

void AHeroCharacter::CheckSelf(bool res, FString msg)
{
	if(!res)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, HeroName + TEXT(" ") + msg);
	}
}

float AHeroCharacter::GetSkillCDPercent(int32 n)
{
	if(n > 0 && n < Skill_Amount)
	{
		if(Skill_CDing[n])
		{
			return Skill_CurrentCD[n] / Skill_MaxCD[n];
		}
	}
	return 1.f;
}

float AHeroCharacter::GetHPPercent()
{
	return CurrentHP / CurrentMaxHP;
}

float AHeroCharacter::GetMPPercent()
{
	return CurrentMP / CurrentMaxMP;
}

void AHeroCharacter::UpdateHPMPAS()
{
	AAONGameState* ags = Cast<AAONGameState>(UGameplayStatics::GetGameState(GetWorld()));
	CurrentMaxHP = BaseHP + BaseStrength * ags->StrengthToHP;
	CurrentHealingHP = BaseHealingHP + BaseStrength * ags->StrengthToHealingHP;
	CurrentMaxMP = BaseMP + BaseIntelligence * ags->IntelligenceToMP;
	CurrentHealingMP = BaseHealingMP + BaseIntelligence * ags->IntelligenceToHealingMP;

	CurrentAttackSpeed = BaseMP + BaseAgility * ags->AgilityToAttackSpeed;
	CurrentAttackSpeedSecond = BaseAttackSpeedSecond / (1 + CurrentAttackSpeed * 0.01);
	CurrentArmor = BaseArmor + BaseAgility * ags->AgilityToDefense;
}

void AHeroCharacter::UpdateSAI()
{
	if(CurrentLevel <= LevelProperty_Strength.Num())
	{
		Strength = BaseStrength + LevelProperty_Strength[CurrentLevel - 1];
	}
	else if(LevelProperty_Strength.Num() > 0)
	{
		Strength = BaseStrength + LevelProperty_Strength.Last();
	}
	if(CurrentLevel <= LevelProperty_Agility.Num())
	{
		Agility = BaseAgility + LevelProperty_Agility[CurrentLevel - 1];
	}
	else if(LevelProperty_Intelligence.Num() > 0)
	{
		Agility = BaseAgility + LevelProperty_Agility.Last();
	}
	if(CurrentLevel <= LevelProperty_Intelligence.Num())
	{
		Intelligence = BaseIntelligence + LevelProperty_Intelligence[CurrentLevel - 1];
	}
	else if(LevelProperty_Intelligence.Num() > 0)
	{
		Intelligence = BaseIntelligence + LevelProperty_Intelligence.Last();
	}
}

bool AHeroCharacter::ShowSkillHint(int32 index)
{
	if (CurrentSkillHint)
	{
		CurrentSkillHint->Destroy();
	}
	if(index < Skill_HintActor.Num())
	{
		CurrentSkillHint = GetWorld()->SpawnActor<ASkillHintActor>(Skill_HintActor[index]);
		FVector pos = GetActorLocation();
		if (CurrentSkillHint)
		{
			CurrentSkillIndex = index;
			CurrentSkillHint->SetActorLocation(pos);
		}
		return true;
	}
	return false;
}

void AHeroCharacter::HideSkillHint()
{
	if (CurrentSkillHint)
	{
		CurrentSkillHint->Destroy();
	}
	CurrentSkillHint = NULL;
}

bool AHeroCharacter::UseSkill(int32 index, FVector VFaceTo, FVector Pos)
{
	if (index < 0)
	{
		index = CurrentSkillIndex;
	}
	if (Skill_FaceSkill.Num() > index)
	{
		if (Skill_FaceSkill[index])
		{
			FVector dir = Pos - GetActorLocation();
			SetActorRotation(dir.Rotation());
		}
	}
	BP_ImplementSkill(index, VFaceTo, Pos);
	return true;
}

int32 AHeroCharacter::GetCurrentSkillIndex()
{
	return CurrentSkillIndex;
}

bool AHeroCharacter::CheckCurrentActionFinish()
{
	switch (BodyStatus)
	{
	case EHeroBodyStatus::Standing:
	{
		switch (CurrentAction.ActionStatus)
		{
		case EHeroActionStatus::Default:
			break;
		case EHeroActionStatus::MoveToPosition:
			break;
		case EHeroActionStatus::MoveToActor:
			break;
		case EHeroActionStatus::FollowActor:
			break;
		case EHeroActionStatus::AttackActor:
			break;
		case EHeroActionStatus::MovingAttackActor:
			break;
		case EHeroActionStatus::MoveAndAttack:
			break;
		case EHeroActionStatus::SpellToPosition:
			break;
		case EHeroActionStatus::SpellToActor:
			break;
		case EHeroActionStatus::SpellToDirection:
			return true;
			break;
		case EHeroActionStatus::SpellToSelf:
			break;
		case EHeroActionStatus::MoveToPickup:
			break;
		case EHeroActionStatus::MoveToThrowEqu:
			break;
		case EHeroActionStatus::ThrowEquToActor:
			break;
		default:
			break;
		}
	}
	break;
	case EHeroBodyStatus::Moving:
	{
		// 移動到夠接近就 Pop 掉
		float Distance = FVector::Dist(CurrentAction.TargetVec1, this->GetActorLocation());
		if (Distance < MinimumDontMoveDistance && this->GetVelocity().Size() < 5)
		{
			return true;
		}
	}
	break;
	case EHeroBodyStatus::Stunning:
		break;
	case EHeroBodyStatus::AttackWating:
	{
		if (CurrentAction.TargetActor &&
		        CurrentAction.TargetActor->CurrentHP <= 0)
		{
			return true;
		}
	}
	break;
	case EHeroBodyStatus::AttackBegining:
	{
		if (CurrentAction.TargetActor &&
		        CurrentAction.TargetActor->CurrentHP <= 0)
		{
			return true;
		}
	}
	break;
	case EHeroBodyStatus::AttackEnding:
	{
		if (CurrentAction.TargetActor &&
		        CurrentAction.TargetActor->CurrentHP <= 0)
		{
			return true;
		}
	}
	break;
	case EHeroBodyStatus::SpellBegining:
		break;
	case EHeroBodyStatus::SpellEnding:
		break;
	default:
		break;
	}
	return false;
}

void AHeroCharacter::DoAction(const FHeroAction& CurrentAction)
{
	switch (CurrentAction.ActionStatus)
	{
	case EHeroActionStatus::Default:
		PopAction();
		break;
	case EHeroActionStatus::MoveToPosition:
		DoAction_MoveToPosition(CurrentAction);
		break;
	case EHeroActionStatus::MoveToActor:
		break;
	case EHeroActionStatus::FollowActor:
		break;
	case EHeroActionStatus::AttackActor:
		DoAction_AttackActor(CurrentAction);
		break;
	case EHeroActionStatus::MovingAttackActor:
		break;
	case EHeroActionStatus::MoveAndAttack:
		break;
	case EHeroActionStatus::SpellToPosition:
		break;
	case EHeroActionStatus::SpellToActor:
		break;
	case EHeroActionStatus::SpellToDirection:
		DoAction_SpellToDirection(CurrentAction);
		break;
	case EHeroActionStatus::SpellToSelf:
		break;
	case EHeroActionStatus::MoveToPickup:
		break;
	case EHeroActionStatus::MoveToThrowEqu:
		break;
	case EHeroActionStatus::ThrowEquToActor:
		break;
	default:
		break;
	}
}

void AHeroCharacter::DoNothing()
{
	switch (BodyStatus)
	{
	case EHeroBodyStatus::Standing:
		break;
	case EHeroBodyStatus::Moving:
	{
		AAmbitionOfNobunagaPlayerController* acontrol =
		    Cast<AAmbitionOfNobunagaPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
		//AAONGameState* ags = Cast<AAONGameState>(UGameplayStatics::GetGameState(GetWorld()));
		acontrol->CharacterStopMove(this);
		BodyStatus = EHeroBodyStatus::Standing;
	}
	break;
	case EHeroBodyStatus::Stunning:
		break;
	case EHeroBodyStatus::AttackWating:
		break;
	case EHeroBodyStatus::AttackBegining:
		break;
	case EHeroBodyStatus::AttackEnding:
		break;
	case EHeroBodyStatus::SpellBegining:
		break;
	case EHeroBodyStatus::SpellEnding:
		break;
	default:
		break;
	}
}

void AHeroCharacter::DoAction_MoveToPosition(const FHeroAction& CurrentAction)
{
	if (LastMoveTarget != CurrentAction.TargetVec1)
	{
		DoAction_MoveToPositionImpl(CurrentAction);
		LastMoveTarget = CurrentAction.TargetVec1;
	}
}

void AHeroCharacter::DoAction_MoveToPositionImpl(const FHeroAction& CurrentAction)
{
	AAmbitionOfNobunagaPlayerController* acontrol =
	    Cast<AAmbitionOfNobunagaPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
	//AAONGameState* ags = Cast<AAONGameState>(UGameplayStatics::GetGameState(GetWorld()));
	switch (BodyStatus)
	{
	case EHeroBodyStatus::AttackWating:
	case EHeroBodyStatus::AttackBegining:
	case EHeroBodyStatus::AttackEnding:
	case EHeroBodyStatus::Standing:
	{

		float Distance = FVector::Dist(CurrentAction.TargetVec1, this->GetActorLocation());
		if (Distance > MinimumDontMoveDistance)
		{
			acontrol->CharacterMove(this, CurrentAction.TargetVec1);
			BodyStatus = EHeroBodyStatus::Moving;
			GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Magenta,
			                                 FString::Printf(L"Distance:%.1f %.1f", Distance, MinimumDontMoveDistance));
		}
	}
	break;
	case EHeroBodyStatus::Moving:
		if (LastMoveTarget != CurrentAction.TargetVec1)
		{
			acontrol->CharacterMove(this, CurrentAction.TargetVec1);
		}
		break;
	case EHeroBodyStatus::Stunning:
		break;
	case EHeroBodyStatus::SpellBegining:
		BodyStatus = EHeroBodyStatus::Standing;
		break;
	case EHeroBodyStatus::SpellEnding:
		BodyStatus = EHeroBodyStatus::Standing;
		break;
	default:
		break;
	}
}

void AHeroCharacter::PopAction()
{
	if (ActionQueue.Num() > 0)
	{
		ActionQueue.RemoveAt(0);
		if (ActionQueue.Num() > 0)
		{
			CurrentAction = ActionQueue[0];
		}
		else
		{
			CurrentAction.ActionStatus = EHeroActionStatus::Default;
		}
	}
}

void AHeroCharacter::DoAction_AttackActor(const FHeroAction& CurrentAction)
{
	FVector dir = CurrentAction.TargetActor->GetActorLocation() - GetActorLocation();
	SetActorRotation(dir.Rotation());
	switch (BodyStatus)
	{
	case EHeroBodyStatus::Standing:
	{
		float DistanceToTargetActor = FVector::Dist(CurrentAction.TargetActor->GetActorLocation(), this->GetActorLocation());
		if (CurrentAttackRadius > DistanceToTargetActor)
		{
			BodyStatus = EHeroBodyStatus::AttackWating;
			IsAttacked = false;
		}
		else
		{
			AAmbitionOfNobunagaPlayerController* acontrol =
			    Cast<AAmbitionOfNobunagaPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
			//AAONGameState* ags = Cast<AAONGameState>(UGameplayStatics::GetGameState(GetWorld()));
			acontrol->CharacterMove(this, CurrentAction.TargetActor->GetActorLocation());
			BodyStatus = EHeroBodyStatus::Moving;
		}
	}
	break;
	case EHeroBodyStatus::Moving:
	{
		float DistanceToTargetActor = FVector::Dist(CurrentAction.TargetActor->GetActorLocation(), this->GetActorLocation());
		if (CurrentAttackRadius > DistanceToTargetActor)
		{
			AAmbitionOfNobunagaPlayerController* acontrol =
			    Cast<AAmbitionOfNobunagaPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
			//AAONGameState* ags = Cast<AAONGameState>(UGameplayStatics::GetGameState(GetWorld()));
			acontrol->CharacterStopMove(this);
			BodyStatus = EHeroBodyStatus::AttackWating;
			IsAttacked = false;
		}
		else if (FollowActorUpdateCounting > FollowActorUpdateTimeGap)
		{
			FollowActorUpdateCounting = 0;
			AAmbitionOfNobunagaPlayerController* acontrol =
			    Cast<AAmbitionOfNobunagaPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
			//AAONGameState* ags = Cast<AAONGameState>(UGameplayStatics::GetGameState(GetWorld()));
			acontrol->CharacterMove(this, CurrentAction.TargetActor->GetActorLocation());
		}
	}
	break;
	case EHeroBodyStatus::Stunning:
		break;
	case EHeroBodyStatus::AttackWating:
	{
		if (AttackingCounting > CurrentAttackSpeedSecond)
		{
			AttackingCounting = 0;
			BodyStatus = EHeroBodyStatus::AttackBegining;
			PlayAttack = true;
		}
		// 播放攻擊動畫
		// ...
	}
	break;
	case EHeroBodyStatus::AttackBegining:
	{
		if (!IsAttacked && AttackingCounting > CurrentAttackingBeginingTimeLength)
		{
			IsAttacked = true;
			AAONGameState* ags = Cast<AAONGameState>(UGameplayStatics::GetGameState(GetWorld()));
			float Injury = ags->ArmorConvertToInjuryPersent(CurrentAction.TargetActor->CurrentArmor);
			float Damage = this->CurrentAttack * Injury;

			if (HeroBullet)
			{
				FVector pos = GetActorLocation();
				ABulletActor* bullet = GetWorld()->SpawnActor<ABulletActor>(HeroBullet);
				if (bullet)
				{
					bullet->SetActorLocation(pos);
					bullet->SetTartgetActor(CurrentAction.TargetActor);
					bullet->Damage = Damage;
				}
			}
			else
			{
				CurrentAction.TargetActor->CurrentHP -= Damage;
			}
			BodyStatus = EHeroBodyStatus::AttackEnding;
		}
	}
	break;
	case EHeroBodyStatus::AttackEnding:
	{
		if (AttackingCounting > CurrentAttackingBeginingTimeLength + CurrentAttackingEndingTimeLength)
		{
			BodyStatus = EHeroBodyStatus::Standing;
		}
	}
	break;
	case EHeroBodyStatus::SpellBegining:
		break;
	case EHeroBodyStatus::SpellEnding:
		break;
	default:
		break;
	}
}

void AHeroCharacter::DoAction_SpellToDirection(const FHeroAction& CurrentAction)
{
	FVector dir = CurrentAction.TargetVec1;
	SetActorRotation(dir.Rotation());
	switch (BodyStatus)
	{
	case EHeroBodyStatus::Moving:
	{
		AAmbitionOfNobunagaPlayerController* acontrol =
		    Cast<AAmbitionOfNobunagaPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
		acontrol->CharacterStopMove(this);
	}
	// no break;
	case EHeroBodyStatus::Standing:
	{
		BodyStatus = EHeroBodyStatus::SpellWating;
		SpellingCounting = 0;
	}
	break;
	case EHeroBodyStatus::Stunning:
		break;
	case EHeroBodyStatus::AttackWating:
		break;
	case EHeroBodyStatus::AttackBegining:
		break;
	case EHeroBodyStatus::AttackEnding:
		break;
	case EHeroBodyStatus::SpellWating:
	{
		if (SpellingCounting > CurrentSpellingWatingTimeLength)
		{
			BodyStatus = EHeroBodyStatus::SpellBegining;
			SpellingCounting = 0;
		}
	}
	break;
	case EHeroBodyStatus::SpellBegining:
	{
		if (Role == ROLE_Authority)
		{
			if (SpellingCounting > CurrentSpellingBeginingTimeLength)
			{
				if (LastUseSkill != CurrentAction)
				{
					GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Magenta,
					                                 FString::Printf(L"GetVelocity: %.1f %d",
					                                         SpellingCounting, CurrentAction.SequenceNumber));
					BodyStatus = EHeroBodyStatus::SpellEnding;
					SpellingCounting = 0;
					AAmbitionOfNobunagaPlayerController* acontrol =
					    Cast<AAmbitionOfNobunagaPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
					acontrol->HeroUseSkill(this, CurrentAction.TargetIndex1, CurrentAction.TargetVec1, CurrentAction.TargetVec2);
				}
				LastUseSkill = CurrentAction;
			}
		}
	}
	break;
	case EHeroBodyStatus::SpellEnding:
	{
		if (SpellingCounting > CurrentSpellingBeginingTimeLength)
		{
			BodyStatus = EHeroBodyStatus::Standing;
		}
	}
	break;
	default:
		break;
	}
}

void AHeroCharacter::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AHeroCharacter, Equipments);
	DOREPLIFETIME(AHeroCharacter, CurrentHP);
	DOREPLIFETIME(AHeroCharacter, BodyStatus);
	DOREPLIFETIME(AHeroCharacter, ActionQueue);
}
