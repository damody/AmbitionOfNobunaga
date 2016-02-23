 // Fill out your copyright notice in the Description page of Project Settings.

#include "DA_RTSPrivatePCH.h"
#include "HeroBuff.h"




UHeroBuff* UHeroBuff::NewHeroBuff()
{
	return NewObject<UHeroBuff>();
}
