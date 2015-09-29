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
    HeroStatus = EHeroStatusEnum::Stand;
    GetMesh()->SetWorldRotation(FQuat(FRotator(0, -90, 0)));

	// 攻擊動畫播到幾秒時發出攻擊
	AnimationInstantAttack = 0.2;
	// 目前攻擊動畫時間長度
	CurrentAttackTime = 0.5;
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
}

// Called when the game starts or when spawned
void AHeroCharacter::BeginPlay()
{
    Super::BeginPlay();
    GetCapsuleComponent()->OnClicked.AddDynamic(this, &AHeroCharacter::OnMouseClicked);
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
    WantThrow = NULL;
    WantPickup = NULL;
    WantAttack = NULL;
    // 依等級更新力敏智
    UpdateSAI();
    // 依等級更新血魔攻速
    UpdateHPMPAS();
    CurrentHP = CurrentMaxHP;
    CurrentMP = CurrentMaxMP;
    CurrentAttackRadius = BaseAttackRadius;
    CurrentAttack = BaseAttack;
    CurrentArmor = BaseArmor;

}

// Called every frame
void AHeroCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
	if (CurrentHP < 0)
	{
		CurrentHP = 0;
	}
    CurrentAttackSpeedCount += DeltaTime;
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
            HeroStatus = EHeroStatusEnum::Stand;
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
					if (idx == 0)
					{
						Equipments[0]->DetachRootComponentFromParent();
						Equipments[0]->RestoreCollision();
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
            HeroStatus = EHeroStatusEnum::Stand;
        }
    }
    // 打人啦~
    else if(WantAttack)
    {
		FVector dir = WantAttack->GetActorLocation() - GetActorLocation();
		SetActorRotation(dir.Rotation());
        switch(HeroStatus)
        {
        case EHeroStatusEnum::Stand:
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
                HeroStatus = EHeroStatusEnum::AttackBegin;
				IsAttacked = false;
            }
        }
        break;
        case EHeroStatusEnum::Walk:
        {
            if(FVector::Dist(GetActorLocation(), WantAttack->GetActorLocation()) < CurrentAttackRadius)
            {
                GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, HeroName + TEXT(" 打人啦"));
                ARTS_HUD* hud = Cast<ARTS_HUD>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());
                if(hud)
                {
                    hud->StopMovementHero(this);
                }
                HeroStatus = EHeroStatusEnum::AttackBegin;
				IsAttacked = false;
            }
        }
        break;
        case EHeroStatusEnum::AttackBegin:
        {
			// 時間到就攻擊
			if (!IsAttacked && CurrentAttackSpeedCount > AnimationInstantAttack)
			{
				IsAttacked = true;
				AAONGameState* ags = Cast<AAONGameState>(UGameplayStatics::GetGameState(GetWorld()));
				float Injury = ags->ArmorConvertToInjuryPersent(WantAttack->CurrentArmor);
				float Damage = this->CurrentAttack * Injury;

				
				if (HeroBullet)
				{
					FVector pos = GetActorLocation();
					ABulletActor* bullet = GetWorld()->SpawnActor<ABulletActor>(HeroBullet);
					if (bullet)
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
                HeroStatus = EHeroStatusEnum::Attacking;
            }
            // 播放攻擊動畫
            // ...
            PlayAttack = true;
        }
        break;
        case EHeroStatusEnum::Attacking:
        {
            if(CurrentAttackSpeedCount > CurrentAttackTime)
            {
                HeroStatus = EHeroStatusEnum::AttackEnd;
            }
        }
        break;
        case EHeroStatusEnum::AttackEnd:
        {
            // 如果播完了攻擊
			HeroStatus = EHeroStatusEnum::Stand;
        }
        break;
        }
    }
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
	for (int32 idx = 0; idx < Equipments.Num(); ++idx)
	{
		if (Equipments[idx] == equ)
		{
			return false;
		}
	}
    for(int32 idx = 0; idx < Equipments.Num(); ++idx)
    {
        if(Equipments[idx] == NULL)
        {
            Equipments[idx] = equ;
			if (idx == 0)
			{
				Equipments[0]->AttachRootComponentTo(GetMesh(), TEXT("hand_rSocket"), EAttachLocation::SnapToTarget);
				Equipments[0]->IgnoreCollision();
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
            }
            hud->ClickedSelected = true;
            hud->ClearAllSelection();
            SelectionOn();
        }
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

void AHeroCharacter::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(AHeroCharacter, WantPickup);
    DOREPLIFETIME(AHeroCharacter, WantThrow);
    DOREPLIFETIME(AHeroCharacter, Equipments);
    DOREPLIFETIME(AHeroCharacter, ThrowDestination);
    DOREPLIFETIME(AHeroCharacter, WantAttack);
    DOREPLIFETIME(AHeroCharacter, CurrentHP);
    DOREPLIFETIME(AHeroCharacter, HeroStatus);
}
