 // Fill out your copyright notice in the Description page of Project Settings.

#include "DA_RTSPrivatePCH.h"
#include "HeroBuff.h"




UHeroBuff* UHeroBuff::NewHeroBuff()
{
	return NewObject<UHeroBuff>();
}

TArray<UHeroBuff*> UHeroBuff::CloneArray(TArray<UHeroBuff*> input)
{
	TArray<UHeroBuff*> res;
	for (UHeroBuff* hb : input)
	{
		res.Add(hb->Clone());
	}
	return res;
}

UHeroBuff* UHeroBuff::Clone()
{
	UHeroBuff* data = NewObject<UHeroBuff>();
	data->Priority = Priority;
	data->Name = Name;
	data->Head = Head;
	data->Dazzing = Dazzing;
	data->BuffKind = BuffKind;
	data->CanSuperposition = CanSuperposition;
	data->Superposition = Superposition;
	data->Duration = Duration;
	return data;
}
