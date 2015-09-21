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

AHeroCharacter::AHeroCharacter(const FObjectInitializer& ObjectInitializer)
    : Super(FObjectInitializer::Get())
{
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
    GetMesh()->SetWorldRotation(FQuat(FRotator(0, -90, 0)));

	// 基礎血量
	BaseHP = 450;
	// 基礎魔力
	BaseMP = 100;
	// 基礎撿物品距離
    PickupObjectDistance = 500;
	// 基礎等級
	CurrentLevel = 1;
	// 基礎攻速
	BaseAttackSpeed = 1.8;
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
}

// Called every frame
void AHeroCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
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
			WantThrow->ServerSetLocation(ThrowDestination);
            for(int32 idx = 0; idx < Equipments.Num(); ++idx)
            {
                if(Equipments[idx] == WantThrow)
                {
                    Equipments[idx] = NULL;
                }
            }
            WantThrow = NULL;
            // 停止移動
			ARTS_HUD* hud = Cast<ARTS_HUD>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());
			if (hud)
			{
				hud->StopMovementHero(this);
			}
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
    for(int32 idx = 0; idx < Equipments.Num(); ++idx)
    {
        if(Equipments[idx] == NULL)
        {
            Equipments[idx] = equ;
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
        if(hud->CurrentSelection.Num() == 1)
        {
            if(hud->CurrentSelection[0] == this)
            {
                return;
            }
        }
        hud->ClickedSelected = true;
        hud->ClearAllSelection();
    }
    SelectionOn();
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

void AHeroCharacter::UpdateHPMPAS()
{
	AAONGameState* ags = Cast<AAONGameState>(UGameplayStatics::GetGameState(GetWorld()));
	CurrentHP = BaseHP + BaseStrength * ags->StrengthToHP;
	CurrentHealingHP = BaseHealingHP + BaseStrength * ags->StrengthToHealingHP;
	CurrentMP = BaseMP + BaseIntelligence * ags->IntelligenceToMP;
	CurrentHealingMP = BaseHealingMP + BaseIntelligence * ags->IntelligenceToHealingMP;

	CurrentAttackSpeed = BaseMP + BaseAgility * ags->AgilityToAttackSpeed;
	CurrentDefense = BaseDefense + BaseAgility * ags->AgilityToDefense;
}

void AHeroCharacter::UpdateSAI()
{
	if (CurrentLevel <= LevelProperty_Strength.Num())
	{
		Strength = BaseStrength + LevelProperty_Strength[CurrentLevel - 1];
	}
	else
	{
		Strength = BaseStrength + LevelProperty_Strength.Last();
	}
	if (CurrentLevel <= LevelProperty_Agility.Num())
	{
		Agility = BaseAgility + LevelProperty_Agility[CurrentLevel - 1];
	}
	else
	{
		Agility = BaseAgility + LevelProperty_Agility.Last();
	}
	if (CurrentLevel <= LevelProperty_Intelligence.Num())
	{
		Intelligence = BaseIntelligence + LevelProperty_Intelligence[CurrentLevel - 1];
	}
	else
	{
		Intelligence = BaseIntelligence + LevelProperty_Intelligence.Last();
	}
}

void AHeroCharacter::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AHeroCharacter, WantPickup);
	DOREPLIFETIME(AHeroCharacter, WantThrow);
	DOREPLIFETIME(AHeroCharacter, Equipments);
	DOREPLIFETIME(AHeroCharacter, ThrowDestination);
}
