// Fill out your copyright notice in the Description page of Project Settings.


#include "MansionAIController.h"

#include "../MansionCharacter.h"
#include "Hospital_MPIA/GraphManager.h"
#include "Hospital_MPIA/Route/CheckPoint/CheckPointComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Steering/Steering.h"

void AMansionAIController::BeginPlay()
{
	Super::BeginPlay();

	if (!GraphManager)
	{
		GraphManager = GetWorld()->GetSubsystem<UGraphManager>();
	}

	AActor* Actor = GetPawn();
	AMansionCharacter* MansionCharacter = Cast<AMansionCharacter>(Actor);
	if (!MansionCharacter)
	{
		UE_LOG(LogTemp, Warning, TEXT("Controlled Pawn is not a MansionCharacter !"));
	}

	UHospitalDataAsset* HospitalDataAsset = MansionCharacter->HospitalDataAsset;
	if (!HospitalDataAsset)
	{
		UE_LOG(LogTemp, Error, TEXT("Hospital Data Asset not defined in Character's Blueprint !"));
	}

	TargetReached = false;

	/*
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AMansionAIController::DelayedSetupGraph, 0.1f, false);
	*/

	Nearest = GraphManager->GetNearestCheckpoint(GetPawn()->GetActorLocation());

	ChooseRandomCheckpoint();

	Path = GraphManager->FindPath(Nearest, Target);

	if (Path.Num() <= 0)
	{
		UE_LOG(LogTemp, Error, TEXT("Path is empty!"));
	}

	int i = 0;
	for (auto step : Path)
	{
		UE_LOG(LogTemp, Warning, TEXT("Node %d of Path : Component : %s"), i, *step->GetComponentLocation().ToString());
			++i;
	}
}

void AMansionAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Path.Num() <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Path is empty!"));
		return;
	}

	AActor* Actor = GetPawn();
	AMansionCharacter* MansionCharacter = Cast<AMansionCharacter>(Actor);
	if (MansionCharacter)
	{
		Steering Steering;

		if (TargetReached)
		{
			++index;
			TargetReached = false;
		}

		if (index >= Path.Num())
		{
			return;
		}


		PathNode = Path[index];
		FVector Steer;
		if (PathNode == Target)
		{
			Steer = Steering.Arrival(MansionCharacter, this, PathNode->GetComponentLocation());
		}
		else
		{
			Steer = Steering.Seek(MansionCharacter, PathNode->GetComponentLocation());
		}

		if (MansionCharacter->HospitalDataAsset->ShowLog)
		{
			UE_LOG(LogTemp, Warning, TEXT("TargetLocation : %s"),*Target->GetComponentLocation().ToString());
			UE_LOG(LogTemp, Warning, TEXT("ActorLocation : %s"),*MansionCharacter->GetActorLocation().ToString());
			UE_LOG(LogTemp, Warning, TEXT("Steer : %s"),*Steer.ToString());
		}
		
		MansionCharacter->Move(Steer, 1);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Not a MansionCharacter"));
	}
}

void AMansionAIController::ChooseRandomCheckpoint()
{
	if (GraphManager)
	{
		UCheckPointComponent* RandomCheckPoint = GraphManager->GetRandomCheckpoint();
		if (RandomCheckPoint)
		{
			Target = RandomCheckPoint;
			UE_LOG(LogTemp, Warning, TEXT("Selected CheckPoint: %s"), *Target->GetComponentLocation().ToString());
		}
	}
}