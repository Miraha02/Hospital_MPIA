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
	
	TargetLocation = GraphManager->GetNearestCheckpoint(GetPawn()->GetActorLocation());
	
}

void AMansionAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AActor* Actor = GetPawn();
	AMansionCharacter* MansionCharacter = Cast<AMansionCharacter>(Actor);
	if (MansionCharacter)
	{
		Steering Steering;
		FVector Steer = Steering.Seek(MansionCharacter, TargetLocation, MansionCharacter->GetActorLocation());

		UE_LOG(LogTemp, Warning, TEXT("TargetLocation : %s"),*TargetLocation.ToString());
		UE_LOG(LogTemp, Warning, TEXT("ActorLocation : %s"),*MansionCharacter->GetActorLocation().ToString());
		UE_LOG(LogTemp, Warning, TEXT("Steer : %s"),*Steer.ToString());
		
		MansionCharacter->Move(Steer, 1);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Not a MansionCharacter"));
	}
}
