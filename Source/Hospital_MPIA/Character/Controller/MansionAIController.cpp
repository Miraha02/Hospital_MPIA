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
		GraphManager = new class GraphManager;
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

	// Setup le graph de checkpoint
	GraphManager->SetupGraph();
	
	Nearest = GraphManager->GetNearestCheckpoint(GetPawn()->GetActorLocation());

	ChooseRandomCheckpoint();

	Path = GraphManager->FindPath(Nearest, Target);

	if (Path.Num() <= 0)
	{
		UE_LOG(LogTemp, Error, TEXT("Path is empty!"));
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
		UCheckPointComponent* ActualTarget = Path[index];
		FVector Steer;
		if (ActualTarget == Target)
		{
			Steer = Steering.Arrival(MansionCharacter, this, ActualTarget->GetComponentLocation());
		}
		else
		{
			Steer = Steering.Seek(MansionCharacter, ActualTarget->GetComponentLocation());
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