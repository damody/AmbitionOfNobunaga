// Fill out your copyright notice in the Description page of Project Settings.

#include "AmbitionOfNobunaga.h"
#include "SkillHintActor.h"
#include "PaperSprite.h"


ASkillHintActor::ASkillHintActor(const FObjectInitializer& ObjectInitializer)
    : Super(FObjectInitializer::Get())
{
    bReplicates = false;
    PrimaryActorTick.bCanEverTick = false;
    Scene = ObjectInitializer.CreateDefaultSubobject<USceneComponent>(this, TEXT("root0"));
    BodySprite = ObjectInitializer.CreateDefaultSubobject<UPaperSpriteComponent>(this, TEXT("VisualizeBodySprite0"));
    HeadSprite = ObjectInitializer.CreateDefaultSubobject<UPaperSpriteComponent>(this, TEXT("VisualizeHeadSprite0"));
    FootSprite = ObjectInitializer.CreateDefaultSubobject<UPaperSpriteComponent>(this, TEXT("VisualizeFootSprite0"));
    RootComponent = Scene;
    if(BodySprite)
    {
        BodySprite->AttachParent = Scene;
        BodySprite->AlwaysLoadOnClient = true;
        BodySprite->AlwaysLoadOnServer = false;
        BodySprite->bOwnerNoSee = false;
        BodySprite->bAffectDynamicIndirectLighting = true;
        BodySprite->PrimaryComponentTick.TickGroup = TG_PrePhysics;
        BodySprite->bGenerateOverlapEvents = false;
        BodySprite->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);
        BodySprite->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
        BodySprite->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Ignore);
        BodySprite->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Ignore);
        BodySprite->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
        BodySprite->SetCollisionResponseToChannel(ECC_PhysicsBody, ECR_Ignore);
        BodySprite->SetCollisionResponseToChannel(ECC_Vehicle, ECR_Ignore);
        BodySprite->SetCollisionResponseToChannel(ECC_Destructible, ECR_Ignore);
        // FRotator = rotation Y Z X
        BodySprite->SetWorldRotation(FQuat(FRotator(0, -90, -90)));
    }
    if(HeadSprite)
    {
        HeadSprite->AttachParent = Scene;
        HeadSprite->AlwaysLoadOnClient = true;
        HeadSprite->AlwaysLoadOnServer = true;
        HeadSprite->bOwnerNoSee = false;
        HeadSprite->bAffectDynamicIndirectLighting = true;
        HeadSprite->PrimaryComponentTick.TickGroup = TG_PrePhysics;
        HeadSprite->bGenerateOverlapEvents = false;
        HeadSprite->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);
        HeadSprite->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
        HeadSprite->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Ignore);
        HeadSprite->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Ignore);
        HeadSprite->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
        HeadSprite->SetCollisionResponseToChannel(ECC_PhysicsBody, ECR_Ignore);
        HeadSprite->SetCollisionResponseToChannel(ECC_Vehicle, ECR_Ignore);
        HeadSprite->SetCollisionResponseToChannel(ECC_Destructible, ECR_Ignore);
        // FRotator = rotation Y Z X
        HeadSprite->SetWorldRotation(FQuat(FRotator(0, -90, -90)));
    }
    if(FootSprite)
    {
        FootSprite->AttachParent = Scene;
        FootSprite->AlwaysLoadOnClient = true;
        FootSprite->AlwaysLoadOnServer = true;
        FootSprite->bOwnerNoSee = false;
        FootSprite->bAffectDynamicIndirectLighting = true;
        FootSprite->PrimaryComponentTick.TickGroup = TG_PrePhysics;
        FootSprite->bGenerateOverlapEvents = false;
        FootSprite->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);
        FootSprite->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
        FootSprite->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Ignore);
        FootSprite->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Ignore);
        FootSprite->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
        FootSprite->SetCollisionResponseToChannel(ECC_PhysicsBody, ECR_Ignore);
        FootSprite->SetCollisionResponseToChannel(ECC_Vehicle, ECR_Ignore);
        FootSprite->SetCollisionResponseToChannel(ECC_Destructible, ECR_Ignore);
        // FRotator = rotation Y Z X
        FootSprite->SetWorldRotation(FQuat(FRotator(0, -90, -90)));
    }

    MinimalLength = 100;
    SkillLength = 500;
    IsFixdLength = true;
    SkillType = ESkillHintEnum::NoneHint;
    UseDirectionSkill = false;
	UseRangeSkill = false;
}
#if WITH_EDITOR
void ASkillHintActor::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
    if(SkillLength < MinimalLength)
    {
        SkillLength = MinimalLength;
    }
	UseDirectionSkill = false;
	UseRangeSkill = false;
	switch (SkillType)
	{
	case ESkillHintEnum::DirectionSkill:
	case ESkillHintEnum::DirectionSkill_CanBlock:
	{
		UseDirectionSkill = true;
	}
	break;
	case ESkillHintEnum::RangeSkill:
	{
		UseRangeSkill = true;
	}
	break;
	}
    UpdateLength();
    Super::PostEditChangeProperty(PropertyChangedEvent);
}

void ASkillHintActor::PostInitProperties()
{
    Super::PostInitProperties();
    UpdateLength();
}
#endif
void ASkillHintActor::SetLength(float len)
{
    SkillLength = len;
    UpdateLength();
}

#if WITH_EDITOR
void ASkillHintActor::UpdateLength()
{
    switch(SkillType)
    {
    case ESkillHintEnum::DirectionSkill:
    case ESkillHintEnum::DirectionSkill_CanBlock:
    {
        if(FootSprite->GetSprite() && BodySprite->GetSprite() && HeadSprite->GetSprite())
        {
            float footwidth = FootSprite->GetSprite()->GetSourceSize().Y;
            float headwidth = HeadSprite->GetSprite()->GetSourceSize().Y;
            FVector2D size = BodySprite->GetSprite()->GetSourceSize();
            float scale = (SkillLength - headwidth - footwidth) / size.Y;
            BodySprite->SetWorldScale3D(FVector(1, 1, scale));
            BodySprite->SetRelativeLocation(FVector(footwidth, 0, 0));
            HeadSprite->SetRelativeLocation(FVector(SkillLength - headwidth, 0, 0));
        }
        else if(BodySprite->GetSprite() && HeadSprite->GetSprite())
        {
            float headwidth = HeadSprite->GetSprite()->GetSourceSize().Y;
            FVector2D size = BodySprite->GetSprite()->GetSourceSize();
            float scale = (SkillLength - headwidth) / size.Y;
            BodySprite->SetWorldScale3D(FVector(1, 1, scale));
            HeadSprite->SetRelativeLocation(FVector(SkillLength - headwidth, 0, 0));
        }
        else if(BodySprite->GetSprite())
        {
            FVector2D size = BodySprite->GetSprite()->GetSourceSize();
            float scale = (SkillLength) / size.Y;
            BodySprite->SetWorldScale3D(FVector(1, 1, scale));
        }
    }
    break;
    case ESkillHintEnum::RangeSkill:
    {
		if (BodySprite->GetSprite())
		{
			FVector2D size = BodySprite->GetSprite()->GetSourceSize();
			float scale = (SkillDiameter) / size.X;
			BodySprite->SetWorldScale3D(FVector(scale, 1, scale));
		}
    }
    break;
    }
}
#else
void ASkillHintActor::UpdateLength()
{
}
#endif

void ASkillHintActor::UpdatePos(FVector PlayerPos, FVector MousePos)
{
    FVector dir = MousePos - PlayerPos;
    dir.Z = 0;
	float len = dir.Size();
	dir.Normalize();
    switch(SkillType)
    {
    case ESkillHintEnum::DirectionSkill:
    case ESkillHintEnum::DirectionSkill_CanBlock:
    {
        SetActorRotation(dir.Rotation());
        SetActorLocation(PlayerPos);
		if (len < MinimalLength)
		{
			len = MinimalLength;
		}
		else if (len > SkillLength)
		{
			len = SkillLength;
		}
        if(!IsFixdLength)
        {
            SkillLength = len;
            SkillPos = MousePos;
        }
    }
    break;
	case ESkillHintEnum::RangeSkill:
	{
		if (len < MinimalCastRadius)
		{
			len = MinimalCastRadius;
		}
		else if (len > SkillCastRadius)
		{
			len = SkillCastRadius;
		}
		FVector pos = dir*len + PlayerPos;
		SetActorLocation(pos);
	}
	break;
    }
    UpdateLength();
}

