// Fill out your copyright notice in the Description page of Project Settings.

#include "AmbitionOfNobunaga.h"
#include "HeroCharacter.h"


// Sets default values
AHeroCharacter::AHeroCharacter()
{
    // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;
	Skill_CDing_1 = false;
	Skill_CDing_2 = false;
	Skill_CDing_3 = false;
	Skill_CDing_4 = false;
}

// Called when the game starts or when spawned
void AHeroCharacter::BeginPlay()
{
    Super::BeginPlay();
	Skill_MaxCD_1 = Skill_BaseCD_1;
	Skill_MaxCD_2 = Skill_BaseCD_2;
	Skill_MaxCD_3 = Skill_BaseCD_3;
	Skill_MaxCD_4 = Skill_BaseCD_4;
	Skill_CurrentCD_1 = Skill_BaseCD_1;
	Skill_CurrentCD_2 = Skill_BaseCD_2;
	Skill_CurrentCD_3 = Skill_BaseCD_3;
	Skill_CurrentCD_4 = Skill_BaseCD_4;
}

// Called every frame
void AHeroCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if(Skill_CDing_1)
    {
        Skill_CurrentCD_1 += DeltaTime;
		if (Skill_CurrentCD_1 > Skill_MaxCD_1)
		{
			Skill_CurrentCD_1 = Skill_MaxCD_1;
			Skill_CDing_1 = false;
		}
    }
	if (Skill_CDing_2)
	{
		Skill_CurrentCD_2 += DeltaTime;
		if (Skill_CurrentCD_2 > Skill_MaxCD_2)
		{
			Skill_CurrentCD_2 = Skill_MaxCD_2;
			Skill_CDing_2 = false;
		}
	}
	if (Skill_CDing_3)
	{
		Skill_CurrentCD_3 += DeltaTime;
		if (Skill_CurrentCD_3 > Skill_MaxCD_3)
		{
			Skill_CurrentCD_3 = Skill_MaxCD_3;
			Skill_CDing_3 = false;
		}
	}
	if (Skill_CDing_4)
	{
		Skill_CurrentCD_4 += DeltaTime;
		if (Skill_CurrentCD_4 > Skill_MaxCD_4)
		{
			Skill_CurrentCD_4 = Skill_MaxCD_4;
			Skill_CDing_4 = false;
		}
	}
}

// Called to bind functionality to input
void AHeroCharacter::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
    Super::SetupPlayerInputComponent(InputComponent);

}

void AHeroCharacter::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	if (Skill_LevelCDs1.Num() > 0)
	{
		Skill_BaseCD_1 = Skill_LevelCDs1[0];
	}
	if (Skill_LevelCDs2.Num() > 0)
	{
		Skill_BaseCD_2 = Skill_LevelCDs2[0];
	}
	if (Skill_LevelCDs3.Num() > 0)
	{
		Skill_BaseCD_3 = Skill_LevelCDs3[0];
	}
	if (Skill_LevelCDs4.Num() > 0)
	{
		Skill_BaseCD_4 = Skill_LevelCDs4[0];
	}
	Skill_MaxCD_1 = Skill_BaseCD_1;
	Skill_MaxCD_2 = Skill_BaseCD_2;
	Skill_MaxCD_3 = Skill_BaseCD_3;
	Skill_MaxCD_4 = Skill_BaseCD_4;
	Super::PostEditChangeProperty(PropertyChangedEvent);
}

float AHeroCharacter::GetSkillCD1()
{
	if (Skill_CDing_1)
	{
		return Skill_CurrentCD_1 / Skill_MaxCD_1;
	}
	return 1.f;
}

float AHeroCharacter::GetSkillCD2()
{
	if (Skill_CDing_2)
	{
		return Skill_CurrentCD_2 / Skill_MaxCD_2;
	}
	return 1.f;
}

float AHeroCharacter::GetSkillCD3()
{
	if (Skill_CDing_3)
	{
		return Skill_CurrentCD_3 / Skill_MaxCD_3;
	}
	return 1.f;
}

float AHeroCharacter::GetSkillCD4()
{
	if (Skill_CDing_4)
	{
		return Skill_CurrentCD_4 / Skill_MaxCD_4;
	}
	return 1.f;
}

