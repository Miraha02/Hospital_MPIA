// Fill out your copyright notice in the Description page of Project Settings.


#include "RouteStraight.h"

// Sets default values
ARouteStraight::ARouteStraight()
{
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ARouteStraight::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARouteStraight::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

TArray<UCheckPointComponent*> ARouteStraight::GetCheckPointComponents() const
{
	return CheckpointComponents;
}

TArray<FVector> ARouteStraight::GetCheckPointLocation() const
{
	return TArray<FVector>();
}
