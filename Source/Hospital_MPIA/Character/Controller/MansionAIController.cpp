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

	GameMode = GetWorld()->GetAuthGameMode<AHospitalGameModeBase>();
	if (!GameMode)
	{
		UE_LOG(LogTemp, Error, TEXT("Error when getting GameMode"));
		return;
	}

	TargetReached = false;

	/*
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AMansionAIController::DelayedSetupGraph, 0.1f, false);
	*/

	Nearest = GraphManager->GetNearestCheckpoint(GetPawn()->GetActorLocation());


	SelectNearestTarget();

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
		
		

		// Normalisation de la direction (assure que la longueur reste 1)
		FVector Direction = FVector(Steer.X, Steer.Y, 0.0f).GetSafeNormal();

		if (!Direction.IsNearlyZero())
		{
			// Utiliser la direction du Steering pour avancer
			MansionCharacter->Move(Direction, 1);

			float CurrentYaw = MansionCharacter->GetActorRotation().Yaw;
			float DesiredYaw = Direction.Rotation().Yaw;

			// Trouver l'angle optimal de rotation
			float TurnValue = FMath::FindDeltaAngleDegrees(CurrentYaw, DesiredYaw);

			// Appliquer le TurningFactor pour limiter la vitesse de rotation
			float RotationStep = FMath::Clamp(TurnValue * MansionCharacter->HospitalDataAsset->TurningFactor, -5.0f, 5.0f);

			FRotator NewRotation = MansionCharacter->GetActorRotation();
			NewRotation.Yaw += RotationStep;
			MansionCharacter->SetActorRotation(NewRotation);

			if (MansionCharacter->HospitalDataAsset->ShowLog)
			{
				UE_LOG(LogTemp, Warning, TEXT("TargetLocation : %s"), *Target->GetComponentLocation().ToString());
				UE_LOG(LogTemp, Warning, TEXT("ActorLocation : %s"), *MansionCharacter->GetActorLocation().ToString());
				UE_LOG(LogTemp, Warning, TEXT("Steer : %s"), *Steer.ToString());
				UE_LOG(LogTemp, Warning, TEXT("Direction : %s"), *Direction.ToString());
				UE_LOG(LogTemp, Warning, TEXT("TurnValue : %.6f"), TurnValue);
				UE_LOG(LogTemp, Warning, TEXT("RotationStep : %.6f"), RotationStep);
			}
		}


	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Not a MansionCharacter"));
	}
}

void AMansionAIController::SelectNearestTarget()
{
	int minSize = INT_MAX;

	for (int i = 0; i < GameMode->NB_Patient; ++i)
	{
		TArray<UCheckPointComponent*> PathTmp = GraphManager->FindPath(Nearest, GameMode->ComponentsTarget[i]);
		
		int size = PathTmp.Num();
		if (size == 0)
		{
			UE_LOG(LogTemp, Error, TEXT("Path is empty in SelectNearestTarget Function !"));
			return;
		}

		if (size < minSize)
		{
			UE_LOG(LogTemp, Warning, TEXT("Shortest Target Find !"));
			Path = PathTmp;
			Target = GameMode->ComponentsTarget[i];
		}
	}
}