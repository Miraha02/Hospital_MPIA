// Fill out your copyright notice in the Description page of Project Settings.


#include "MansionAIController.h"

#include "../MansionCharacter.h"
#include "Hospital_MPIA/GraphManager.h"
#include "Hospital_MPIA/Route/CheckPoint/CheckPointComponent.h"
#include "Hospital_MPIA/Route/CheckPoint/HospitalCheckPointComponent.h"
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


	FindAllies();


	SetTargetPath();
}

void AMansionAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AActor* Actor = GetPawn();
	AMansionCharacter* MansionCharacter = Cast<AMansionCharacter>(Actor);
	if (!MansionCharacter)
	{
		UE_LOG(LogTemp, Warning, TEXT("Character is not a Mansion Character !!!"))
	}

	if (!MansionCharacter->HasPatient)
	{
		++tick;
		if (tick == 250)
		{
			SetTargetPath();
			tick = 0;
		}
	}
	
	if (Path.Num() <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Path is empty!"));
		return;
	}
	
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

	MansionCharacter->Move(Direction,1);

	FRotator NewRotation = Direction.Rotation();

	// Appliquer la rotation et d�placer l'acteur
	MansionCharacter->SetActorRotation(NewRotation);
	
	if (MansionCharacter->HospitalDataAsset->ShowLog)
	{
		UE_LOG(LogTemp, Warning, TEXT("TargetLocation : %s"), *Target->GetComponentLocation().ToString());
		UE_LOG(LogTemp, Warning, TEXT("ActorLocation : %s"), *MansionCharacter->GetActorLocation().ToString());
		UE_LOG(LogTemp, Warning, TEXT("Steer : %s"), *Steer.ToString());
		UE_LOG(LogTemp, Warning, TEXT("Direction : %s"), *Direction.ToString());
	}
}

void AMansionAIController::SelectNearestTarget()
{
	if (!GameMode || GameMode->ComponentsTarget.Num() == 0 || !Nearest)
	{
		UE_LOG(LogTemp, Error, TEXT("GameMode, ComponentsTarget ou Nearest est invalide !"));
		return;
	}

	UCheckPointComponent* BestTarget = nullptr;
	float MinDistance = FLT_MAX;

	for (UCheckPointComponent* Checkpoint : GameMode->ComponentsTarget)
	{
		if (!Checkpoint) continue;

		float MyDistance = FVector::Dist(Nearest->GetComponentLocation(), Checkpoint->GetComponentLocation());
		bool IsClosest = true;

		for (auto Ally : Allies)
		{

			if (Ally->HasPatient)
			{
				continue;
			}
			float AllyDistance = FVector::Dist(Ally->GetActorLocation(), Checkpoint->GetComponentLocation());

			// Si un allié est plus proche, on ignore ce checkpoint
			if (AllyDistance < MyDistance)
			{
				IsClosest = false;
				break;
			}
		}

		// Si je suis le plus proche, je prends ce checkpoint
		if (IsClosest && MyDistance < MinDistance)
		{
			MinDistance = MyDistance;
			BestTarget = Checkpoint;
		}
	}

	if (BestTarget)
	{
		Target = BestTarget;
		UE_LOG(LogTemp, Warning, TEXT("Nouvelle cible assignée : %s"), *Target->GetComponentLocation().ToString());
		Path = GraphManager->FindPath(Nearest, Target);
		index = 1;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Aucune cible assignée, toutes sont prises !"));
	}
}


void AMansionAIController::FindAllies()
{
	UWorld* World = GetWorld();
	if (!World) return;

	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(World, AMansionCharacter::StaticClass(), FoundActors);

	Allies.Empty();
	for (AActor* Actor : FoundActors)
	{
		AMansionCharacter* MansionCharacter = Cast<AMansionCharacter>(Actor);
		if (MansionCharacter && MansionCharacter != GetPawn()) // Ne pas s'ajouter soi-même
		{
			Allies.Add(MansionCharacter);
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("%d alliés trouvés !"), Allies.Num());
}


void AMansionAIController::SetTargetPath()
{
	Nearest = GraphManager->GetNearestCheckpoint(GetPawn()->GetActorLocation());

	SelectNearestTarget();

	if (Path.Num() <= 0)
	{
		UE_LOG(LogTemp, Error, TEXT("Path is empty!"));
	}

	AActor* Actor = GetPawn();
	AMansionCharacter* MansionCharacter = Cast<AMansionCharacter>(Actor);
	if (MansionCharacter->HospitalDataAsset->ShowLog)
	{
		int i = 0;
		for (auto step : Path)
		{
			UE_LOG(LogTemp, Warning, TEXT("Node %d of Path : Component : %s"), i, *step->GetComponentLocation().ToString());
			++i;
		}
	}
}

void AMansionAIController::SetHospitalPath()
{
    Nearest = GraphManager->GetNearestCheckpoint(GetPawn()->GetActorLocation());

    if (!Nearest)
    {
        UE_LOG(LogTemp, Error, TEXT("Aucun Checkpoint trouvé à proximité !"));
        return;
    }

    UWorld* World = GetWorld();
    if (!World)
    {
        UE_LOG(LogTemp, Error, TEXT("Impossible de récupérer le monde !"));
        return;
    }

    TArray<AActor*> AllActors;
    UGameplayStatics::GetAllActorsOfClass(World, AActor::StaticClass(), AllActors);

    UCheckPointComponent* BestHospitalCheckpoint = nullptr;
    float MinDistance = FLT_MAX;

    for (AActor* Actor : AllActors)
    {
        TArray<UCheckPointComponent*> CheckPoints;
        Actor->GetComponents<UCheckPointComponent>(CheckPoints);

        for (UCheckPointComponent* Checkpoint : CheckPoints)
        {
            UHospitalCheckPointComponent* HospitalCheckpoint = Cast<UHospitalCheckPointComponent>(Checkpoint);
            if (!HospitalCheckpoint) continue;

            float Distance = FVector::Dist(Nearest->GetComponentLocation(), HospitalCheckpoint->GetComponentLocation());

            if (Distance < MinDistance)
            {
                MinDistance = Distance;
                BestHospitalCheckpoint = HospitalCheckpoint;
            }
        }
    }

    if (BestHospitalCheckpoint)
    {
        Target = BestHospitalCheckpoint;
        UE_LOG(LogTemp, Warning, TEXT("Nouvelle cible assignée : %s"), *Target->GetComponentLocation().ToString());

        Path = GraphManager->FindPath(Nearest, Target);
        index = 1;
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Aucun HospitalCheckPointComponent trouvé !"));
    }

    if (Path.Num() <= 0)
    {
        UE_LOG(LogTemp, Error, TEXT("Path is empty!"));
    }

    // Log pour voir les étapes du chemin
    AActor* Actor = GetPawn();
    AMansionCharacter* MansionCharacter = Cast<AMansionCharacter>(Actor);
    if (MansionCharacter && MansionCharacter->HospitalDataAsset->ShowLog)
    {
        int i = 0;
        for (auto step : Path)
        {
            UE_LOG(LogTemp, Warning, TEXT("Node %d of Path : Component : %s"), i, *step->GetComponentLocation().ToString());
            ++i;
        }
    }
}
