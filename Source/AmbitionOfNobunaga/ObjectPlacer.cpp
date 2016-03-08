// Fill out your copyright notice in the Description page of Project Settings.

#include "AmbitionOfNobunaga.h"
#include "ObjectPlacer.h"

// Sets default values
AObjectPlacer::AObjectPlacer()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AObjectPlacer::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AObjectPlacer::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

void AObjectPlacer::PostEditChangeProperty(struct FPropertyChangedEvent& e)
{
	FName PropertyName = (e.Property != NULL) ? e.Property->GetFName() : NAME_None;
	if (PropertyName == GET_MEMBER_NAME_CHECKED(AObjectPlacer, ClickToUpdate))
	{
		//various uproperty tricks, see link
	}
	if (GWorld)
	{
		static int32 Index = 0;
		FActorSpawnParameters SpawnPar;
		SpawnPar.Name = *FString::Printf(TEXT("Spawned %d"), Index++);
		AActor* Item = GWorld->SpawnActor<AActor>(WantPlacing, SpawnPar);
		if (!Item)
		{
			UE_LOG(LogAmbitionOfNobunaga, Warning, TEXT("Could not spawn."));
		}
		else
		{
			Item->SetActorLocation(GetActorLocation());
		}
	}

	Super::PostEditChangeProperty(e);
}
