// Fill out your copyright notice in the Description page of Project Settings.

#include "AmbitionOfNobunaga.h"
#include <algorithm>
#include "EngineUtils.h"
// for GEngine
#include "Engine.h"
#include "UnrealString.h"
#include "NameTypes.h"
#include "WidgetLayoutLibrary.h"

#include "RTS_HUD.h"
#include "HeroCharacter.h"
#include "AmbitionOfNobunagaPlayerController.h"
#include "Equipment.h"


ARTS_HUD::ARTS_HUD()
{
    localController = NULL;
}

void ARTS_HUD::BeginPlay()
{
    Super::BeginPlay();
    RTSStatus = ERTSStatusEnum::Normal;
    bMouseRButton = false;
    bMouseLButton = false;
    ClickedSelected = false;
    WantPickup = NULL;
    ThrowTexture = NULL;
    for(TActorIterator<AHeroCharacter> ActorItr(GetWorld()); ActorItr; ++ActorItr)
    {
        HeroCanSelection.Add(*ActorItr);
    }
    for(int i = 0; i < 6; ++i)
    {
        FVector2D p1, s1;
        GetEquipmentPosition(i, p1, s1);
        RTS_AddHitBox(p1, s1, FString::Printf(TEXT("Equipment%d"), i + 1), false, 0);
        if(EquipmentMaterial)
        {
            EquipmentDMaterials.Add(UMaterialInstanceDynamic::Create(EquipmentMaterial, this));
        }
    }
    for(int i = 0; i < 4; ++i)
    {
        FVector2D p1, s1;
        GetSkillPosition(i, p1, s1);
        RTS_AddHitBox(p1, s1, FString::Printf(TEXT("Skill%d"), i + 1), false, 0);
        if(SkillMaterial)
        {
            SkillDMaterials.Add(UMaterialInstanceDynamic::Create(SkillMaterial, this));
        }
    }
    if(ThrowMaterial)
    {
        ThrowDMaterial = UMaterialInstanceDynamic::Create(ThrowMaterial, this);
    }
    OnSize();
    bClickHero = false;
    bNeedMouseRDown = false;
    bNeedMouseLDown = false;

}

void ARTS_HUD::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    if(RemoveSelection.Num() > 0)
    {
        RTSStatus = ERTSStatusEnum::Normal;
        for(AHeroCharacter* EachHero : RemoveSelection)
        {
            CurrentSelection.Remove(EachHero);
        }
        RemoveSelection.Empty();
    }
    OnSize();
}

void ARTS_HUD::DrawHUD()
{
    Super::DrawHUD();

    if(RTSStatus == ERTSStatusEnum::Normal && bMouseLButton && IsGameRegion(CurrentMouseXY))
    {
        // selection box
        if(FVector2D::DistSquared(InitialMouseXY, CurrentMouseXY) > 25)
        {
            for(AHeroCharacter* EachHero : HeroCanSelection)
            {
                FVector pos = this->Project(EachHero->GetActorLocation());
                EachHero->ScreenPosition.X = pos.X;
                EachHero->ScreenPosition.Y = pos.Y;
                bool res = CheckInSelectionBox(EachHero->ScreenPosition);
                if(res && !EachHero->isSelection)
                {
                    EachHero->SelectionOn();
                }
                else if(!res && EachHero->isSelection)
                {
                    EachHero->SelectionOff();
                }
            }

            float maxX, maxY;
            float minX, minY;
            maxX = std::max(InitialMouseXY.X, CurrentMouseXY.X);
            maxY = std::max(InitialMouseXY.Y, CurrentMouseXY.Y);
            minX = std::min(InitialMouseXY.X, CurrentMouseXY.X);
            minY = std::min(InitialMouseXY.Y, CurrentMouseXY.Y);
            DrawLine(minX, minY, maxX, minY, SelectionBoxLineColor);
            DrawLine(maxX, minY, maxX, maxY, SelectionBoxLineColor);
            DrawLine(maxX, maxY, minX, maxY, SelectionBoxLineColor);
            DrawLine(minX, maxY, minX, minY, SelectionBoxLineColor);

            DrawRect(SelectionBoxFillColor, minX, minY, maxX - minX - 1, maxY - minY - 1);
        }
    }
    for(AHeroCharacter* EachHero : HeroCanSelection)
    {
        FVector2D headpos = FVector2D(this->Project(EachHero->PositionOnHead->GetComponentLocation()));
        FVector2D footpos = FVector2D(this->Project(EachHero->PositionUnderFoot->GetComponentLocation()));
        footpos.Y += 35;
        float  hpBarLength = EachHero->HPBarLength;
        float  halfHPBarLength = hpBarLength * .5f;
        headpos += HPBarOffset;
        DrawRect(HPBarBackColor, headpos.X - halfHPBarLength - 1, headpos.Y - 1, hpBarLength + 2, HPBarHeight + 2);
        DrawRect(HPBarForeColor, headpos.X - halfHPBarLength, headpos.Y, hpBarLength * EachHero->GetHPPercent(), HPBarHeight);
        float maxhp = EachHero->CurrentMaxHP;
        if(maxhp < 1500)
        {
            for(float i = 100; i < maxhp; i += 100)
            {
                float xpos = headpos.X - halfHPBarLength + hpBarLength * (i / maxhp);
                DrawLine(xpos, headpos.Y, xpos, headpos.Y + HPBarHeight, HPBarBackColor);
            }
        }
        else
        {
            for(float i = 500; i < maxhp; i += 500)
            {
                float xpos = headpos.X - halfHPBarLength + hpBarLength * (i / maxhp);
                DrawLine(xpos, headpos.Y, xpos, headpos.Y + HPBarHeight, HPBarBackColor);
            }
        }
        DrawText(EachHero->HeroName, FLinearColor(1, 1, 1), footpos.X - EachHero->HeroName.Len()*.5f * 15, footpos.Y);
    }
    if(CurrentSelection.Num() > 0)
    {
        if(RTSStatus == ERTSStatusEnum::ThrowEquipment)
        {
            ThrowDMaterial->SetTextureParameterValue(TEXT("InputTexture"), ThrowTexture);
            DrawMaterialSimple(ThrowDMaterial, CurrentMouseXY.X, CurrentMouseXY.Y,
                               100 * ViewportScale, 100 * ViewportScale);
        }
        AHeroCharacter* selectHero = CurrentSelection[0];
        if(SkillMaterial)
        {
            for(int32 idx = 0; idx < 4; ++idx)
            {
                FRTSHitBox* skhb = FindHitBoxByName(FString::Printf(TEXT("Skill%d"), idx + 1));

                if(skhb && SkillDMaterials.Num() > idx && selectHero->Skill_Texture.Num() > idx)
                {
                    SkillDMaterials[idx]->SetTextureParameterValue(TEXT("InputTexture"), selectHero->Skill_Texture[idx]);
                    SkillDMaterials[idx]->SetScalarParameterValue(TEXT("Alpha"), selectHero->GetSkillCDPercent(idx));
                    DrawMaterialSimple(SkillDMaterials[idx], skhb->Coords.X * ViewportScale, skhb->Coords.Y * ViewportScale,
                                       skhb->Size.X * ViewportScale, skhb->Size.Y * ViewportScale);
                }
            }
        }

        if(EquipmentMaterial)
        {
            for(int32 idx = 0; idx < 6; ++idx)
            {
                FRTSHitBox* skhb = FindHitBoxByName(FString::Printf(TEXT("Equipment%d"), idx + 1));

                if(skhb)
                {
                    if(EquipmentDMaterials.Num() > idx && selectHero->Equipments.Num() > idx && selectHero->Equipments[idx])
                    {
                        EquipmentDMaterials[idx]->SetTextureParameterValue(TEXT("InputTexture"), selectHero->Equipments[idx]->Head);
                        EquipmentDMaterials[idx]->SetScalarParameterValue(TEXT("Alpha"), selectHero->GetSkillCDPercent(idx));
                        DrawMaterialSimple(EquipmentDMaterials[idx], skhb->Coords.X * ViewportScale, skhb->Coords.Y * ViewportScale,
                                           skhb->Size.X * ViewportScale, skhb->Size.Y * ViewportScale);
                    }
                    else
                    {
                        DrawRect(SelectionBoxFillColor, skhb->Coords.X * ViewportScale, skhb->Coords.Y * ViewportScale,
                                 skhb->Size.X * ViewportScale, skhb->Size.Y * ViewportScale);
                    }
                }
            }
        }
    }
}

bool ARTS_HUD::CheckInSelectionBox(FVector2D pos)
{
    float maxX, maxY;
    float minX, minY;
    maxX = std::max(InitialMouseXY.X, CurrentMouseXY.X);
    maxY = std::max(InitialMouseXY.Y, CurrentMouseXY.Y);
    minX = std::min(InitialMouseXY.X, CurrentMouseXY.X);
    minY = std::min(InitialMouseXY.Y, CurrentMouseXY.Y);

    if(minX < pos.X && pos.X < maxX &&
            minY < pos.Y && pos.Y < maxY)
    {
        return true;
    }
    return false;
}

void ARTS_HUD::ClearAllSelection()
{
    for(AHeroCharacter* EachHero : CurrentSelection)
    {
        EachHero->SelectionOff();
    }
    CurrentSelection.Empty();
}

FRTSHitBox* ARTS_HUD::FindHitBoxByName(const FString& name)
{
    for(int32 Index = 0; Index < RTS_HitBoxMap.Num(); ++Index)
    {
        if(RTS_HitBoxMap[Index].GetName() == name)
        {
            return &RTS_HitBoxMap[Index];
        }
    }
    return nullptr;
}

void ARTS_HUD::RTS_AddHitBox(FVector2D Position, FVector2D Size, const FString& Name, bool bConsumesInput, int32 Priority)
{
    bool bAdded = false;
    for(int32 Index = 0; Index < RTS_HitBoxMap.Num(); ++Index)
    {
        if(RTS_HitBoxMap[Index].GetPriority() < Priority)
        {
            RTS_HitBoxMap.Insert(FRTSHitBox(Position, Size, Name, bConsumesInput, Priority), Index);
            bAdded = true;
            break;
        }
    }
    if(!bAdded)
    {
        RTS_HitBoxMap.Add(FRTSHitBox(Position, Size, Name, bConsumesInput, Priority));
    }
}

bool ARTS_HUD::IsGameRegion(FVector2D pos)
{
    for(FRTSHitBox& HitBox : RTS_HitBoxMap)
    {
        if(HitBox.Contains(pos, ViewportScale))
        {
            return false;
        }
    }
    return true;
}

bool ARTS_HUD::IsUIRegion(FVector2D pos)
{
    return !IsGameRegion(pos);
}

void ARTS_HUD::AssignSelectionHeroPickup(AEquipment* equ)
{
    if(localController && CurrentSelection.Num() > 0)
    {
        WantPickup = equ;
    }
}

void ARTS_HUD::StopMovementHero(AHeroCharacter* hero)
{
    if(localController)
    {
        localController->AddHeroToStopMoveQueue(hero);
    }
}

void ARTS_HUD::HeroAttack(AHeroCharacter* hero)
{
    bClickHero = true;
    if(localController)
    {
        TArray<AHeroCharacter*> HeroGoAttack;
        for(AHeroCharacter* EachHero : CurrentSelection)
        {
            if(EachHero->TeamId != hero->TeamId)
            {
                HeroGoAttack.Add(EachHero);
            }
        }
        if(HeroGoAttack.Num() > 0)
        {
            localController->AddHeroToAttackQueue(HeroGoAttack, hero);
        }
    }
}

void ARTS_HUD::HeroMove(AHeroCharacter* hero, FVector dst)
{
    if(localController)
    {
        TArray<AHeroCharacter*> oneHero;
        oneHero.Add(hero);
        localController->AddHeroToMoveQueue(dst, oneHero);
    }
}

void ARTS_HUD::ClearHeroWant(AHeroCharacter* hero)
{
    if(localController)
    {
        TArray<AHeroCharacter*> oneHero;
        oneHero.Add(hero);
        localController->AddHeroToClearWantQueue(oneHero);
    }
}

void ARTS_HUD::ShowHeroSkillHint(int32 index)
{
    if(CurrentSelection.Num() > 0)
    {
        CurrentSelection[0]->ShowSkillHint(index);
    }
}

FVector ARTS_HUD::GetCurrentDirection()
{
    FVector dir = CurrentMouseHit - CurrentSelection[0]->GetActorLocation();
    dir.Z = 0;
    dir.Normalize();
    return dir;
}

FRotator ARTS_HUD::GetCurrentRotator()
{
    if(CurrentSelection.Num() > 0)
    {
        FVector dir = CurrentMouseHit - CurrentSelection[0]->GetActorLocation();
        dir.Z = 0;
        return dir.Rotation();
    }
    return FRotator();
}

void ARTS_HUD::OnSize()
{
    ViewportScale = UWidgetLayoutLibrary::GetViewportScale(GetWorld());
}

void ARTS_HUD::OnMouseMove(FVector2D pos, FVector pos3d)
{
    CurrentMouseXY = pos;
    CurrentMouseHit = pos3d;
}

void ARTS_HUD::OnRMouseDown(FVector2D pos)
{
    // hitbox用
    for(FRTSHitBox& HitBox : RTS_HitBoxMap)
    {
        if(HitBox.Contains(pos, ViewportScale))
        {
            RButtonDownHitBox = HitBox.GetName();
            break;
        }
    }
    // 右鍵事件
    if(!bClickHero)
    {
        localController->AddHeroToClearWantQueue(CurrentSelection);
    }
    if(IsGameRegion(pos) && localController && !bClickHero)
    {
        switch(RTSStatus)
        {
        case ERTSStatusEnum::Normal:
        {
            if(CurrentSelection.Num() > 0)
            {
                GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, TEXT("localController->AddHeroToMoveQueue"));
                if(WantPickup)
                {
                    localController->AddHeroToPickupQueue(WantPickup->GetActorLocation(), CurrentSelection[0], WantPickup);
                    WantPickup = NULL;
                }
                else
                {
                    localController->AddHeroToMoveQueue(CurrentMouseHit, CurrentSelection);
                }
            }
        }
        break;
        case ERTSStatusEnum::Move:
            break;
        case ERTSStatusEnum::Attack:
            break;
        case ERTSStatusEnum::ThrowEquipment:
            break;
        case ERTSStatusEnum::SkillHint:
        {
            // 取消技能
            if(IsGameRegion(CurrentMouseXY))
            {
				if (CurrentSelection.Num() > 0)
				{
					CurrentSelection[0]->HideSkillHint();
					RTSStatus = ERTSStatusEnum::Normal;
					localController->AddHeroToMoveQueue(CurrentMouseHit, CurrentSelection);
				}
            }
        }
        default:
            break;
        }
    }
}

void ARTS_HUD::OnRMousePressed1(FVector2D pos)
{
    bClickHero = false;
    ClickStatus = ERTSClickEnum::LastRightClick;
    if(!bMouseRButton)
    {
        bNeedMouseRDown = true;
    }
    bMouseRButton = true;
}

void ARTS_HUD::OnRMousePressed2(FVector2D pos)
{
    if(bNeedMouseRDown)
    {
        bNeedMouseRDown = false;
        OnRMouseDown(pos);
        return;
    }
    bMouseRButton = true;
    // 裝備事件
    for(FRTSHitBox& HitBox : RTS_HitBoxMap)
    {
        if(HitBox.Contains(pos, ViewportScale))
        {
            RTS_HitBoxRButtonPressed(HitBox.GetName());
            if(HitBox.ConsumesInput())
            {
                break;  //Early out if this box consumed the click
            }
        }
    }
    RTS_MouseRButtonPressed();
}

void ARTS_HUD::OnRMouseReleased(FVector2D pos)
{
    // hitbox用
    for(FRTSHitBox& HitBox : RTS_HitBoxMap)
    {
        if(HitBox.Contains(pos, ViewportScale))
        {
            RButtonUpHitBox = HitBox.GetName();
            break;
        }
    }
    bMouseRButton = false;
    // 裝備事件
    for(FRTSHitBox& HitBox : RTS_HitBoxMap)
    {
        if(HitBox.Contains(pos, ViewportScale))
        {
            RTS_HitBoxRButtonReleased(HitBox.GetName());
            if(HitBox.ConsumesInput())
            {
                break;  //Early out if this box consumed the click
            }
        }
    }
    // 如果有點到物品
    if(CurrentSelection.Num() > 0)
    {
        AHeroCharacter* Selection = CurrentSelection[0];
        if(RButtonUpHitBox.Len() > 1 && RButtonUpHitBox == RButtonDownHitBox)
        {
            EquipmentIndex = FCString::Atoi(*RButtonUpHitBox.Right(1)) - 1;
            if(Selection->Equipments[EquipmentIndex])
            {
                RTSStatus = ERTSStatusEnum::ThrowEquipment;
                ThrowTexture = Selection->Equipments[EquipmentIndex]->Head;
            }
            RButtonUpHitBox = RButtonDownHitBox = FString();
        }
    }
    bClickHero = false;
}

void ARTS_HUD::OnLMouseDown(FVector2D pos)
{
    // hitbox用
    for(FRTSHitBox& HitBox : RTS_HitBoxMap)
    {
        if(HitBox.Contains(pos, ViewportScale))
        {
            LButtonDownHitBox = HitBox.GetName();
            break;
        }
    }


}

void ARTS_HUD::OnLMousePressed1(FVector2D pos)
{
    bClickHero = false;
    ClickStatus = ERTSClickEnum::LastLeftClick;
    if(!bMouseLButton)
    {
        bNeedMouseLDown = true;
    }
    bMouseLButton = true;
}

void ARTS_HUD::OnLMousePressed2(FVector2D pos)
{
    if(bNeedMouseLDown)
    {
        bNeedMouseLDown = false;
        OnLMouseDown(pos);
        // 設定SelectionBox初始位置
        if(RTSStatus == ERTSStatusEnum::Normal)
        {
            if(IsGameRegion(CurrentMouseXY)) // 取消選英雄
            {
                InitialMouseXY = pos;
                if(!ClickedSelected)
                {
                    ClearAllSelection();
                }
                ClickedSelected = false;
                UnSelectHero();
            }
        }
        else if(RTSStatus == ERTSStatusEnum::SkillHint) // 放技能
        {
            CurrentSelection[0]->HideSkillHint();
			localController->AddHeroToSkillQueue(CurrentSelection[0], CurrentSelection[0]->GetCurrentSkillIndex(),
				GetCurrentRotator(), GetCurrentDirection(), CurrentMouseHit);
            RTSStatus = ERTSStatusEnum::ToNormal;
        }
        return;
    }
    // 顯示技能提示
    if(CurrentSelection.Num() > 0)
    {
        for(FRTSHitBox& HitBox : RTS_HitBoxMap)
        {
            if(HitBox.GetName().Left(5) == TEXT("Skill"))
            {
                if(HitBox.Contains(pos, ViewportScale))
                {
                    int32 idx = FCString::Atoi(*HitBox.GetName().Right(1)) - 1;
                    bool res = CurrentSelection[0]->ShowSkillHint(idx);
                    if(res)
                    {
                        RTSStatus = ERTSStatusEnum::SkillHint;
                    }
                }
            }
        }
    }
    // 發事件給BP
    for(FRTSHitBox& HitBox : RTS_HitBoxMap)
    {
        if(HitBox.Contains(pos, ViewportScale))
        {
            RTS_HitBoxLButtonPressed(HitBox.GetName());
            if(HitBox.ConsumesInput())
            {
                break;  //Early out if this box consumed the click
            }
        }
    }
}

void ARTS_HUD::OnLMouseReleased(FVector2D pos)
{
    // hitbox用
    for(FRTSHitBox& HitBox : RTS_HitBoxMap)
    {
        if(HitBox.Contains(pos, ViewportScale))
        {
            LButtonUpHitBox = HitBox.GetName();
            break;
        }
    }
    bMouseLButton = false;
    // 選英雄
    if(RTSStatus == ERTSStatusEnum::Normal)
    {
        if(IsGameRegion(CurrentMouseXY))
        {
            if(CurrentSelection.Num() > 0)
            {
                SelectedHero(CurrentSelection[0]);
            }
        }
    }
    // 丟物品
    if(RTSStatus == ERTSStatusEnum::ThrowEquipment)
    {
        GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Cyan, FString::Printf(TEXT("CurrentSelection.Num %d"), CurrentSelection.Num()));
        if(CurrentSelection.Num() > 0)
        {
            GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, TEXT("localController->SetNewMoveDestination"));
            localController->AddHeroToThrowQueue(CurrentMouseHit, CurrentSelection[0], EquipmentIndex);
            RTSStatus = ERTSStatusEnum::Normal;
            ThrowTexture = NULL;
        }
    }
    // 發事件給BP
    for(FRTSHitBox& HitBox : RTS_HitBoxMap)
    {
        if(HitBox.Contains(pos, ViewportScale))
        {
            RTS_HitBoxLButtonReleased(HitBox.GetName());
            if(HitBox.ConsumesInput())
            {
                break;  //Early out if this box consumed the click
            }
        }
    }
    if(RTSStatus == ERTSStatusEnum::ToNormal)
    {
        RTSStatus = ERTSStatusEnum::Normal;
    }
}

