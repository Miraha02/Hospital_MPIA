// Fill out your copyright notice in the Description page of Project Settings.


#include "GraphManager.h"

#include "AITestsCommon.h"
#include "AITestsCommon.h"
#include "AITestsCommon.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
GraphManager::GraphManager()
{

}

void GraphManager::SetupGraph()
{
	UE_LOG(LogTemp, Warning, TEXT("Setting up the graph !"));
	// Récupérer tous les CheckPoints dans la scène
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(FAITestHelpers::GetWorld(), AActor::StaticClass(), FoundActors);

	// Ajouter chaque CheckPoint au graphe
	for (AActor* Actor : FoundActors)
	{
		if (UCheckPointComponent* CheckPoint = Actor->FindComponentByClass<UCheckPointComponent>())
		{
			Graph.Add(CheckPoint, FGraphNode(CheckPoint));
		}
	}

	// Ajouter les connexions entre les CheckPoints
	for (auto& Elem : Graph)
	{
		UCheckPointComponent* CheckPoint = Elem.Key;
		FGraphNode& Node = Elem.Value;

		// Ajouter les voisins connectés
		for (UCheckPointComponent* Neighbor : CheckPoint->ConnectedCheckPoints)
		{
			Node.Neighbors.Add(Neighbor);
		}
	}
}

TArray<UCheckPointComponent*> GraphManager::FindPath(UCheckPointComponent* Start, UCheckPointComponent* Goal)
{
	if (!Graph.Contains(Start) || !Graph.Contains(Goal))
		return {};

	// Dijkstra: Initialisation
	TMap<UCheckPointComponent*, float> GScore;  // Distance minimale trouvée
	TMap<UCheckPointComponent*, UCheckPointComponent*> CameFrom; // Chemin pour reconstruire la route

	// Priority queue (Min-Heap avec TArray + tri)
	TArray<UCheckPointComponent*> OpenSet;
	OpenSet.Add(Start);
	GScore.Add(Start, 0);

	while (OpenSet.Num() > 0)
	{
		// Trouver le noeud avec le plus petit GScore
		OpenSet.Sort([&](const UCheckPointComponent& A, const UCheckPointComponent& B) {
			return GScore[&A] < GScore[&B];
			});

		UCheckPointComponent* Current = OpenSet[0];
		OpenSet.RemoveAt(0);

		// Si on atteint la cible, reconstruire le chemin
		if (Current == Goal)
		{
			TArray<UCheckPointComponent*> Path;
			while (Current)
			{
				Path.Insert(Current, 0);
				Current = CameFrom.Contains(Current) ? CameFrom[Current] : nullptr;
			}
			return Path;
		}

		// Parcourir les voisins
		for (UCheckPointComponent* Neighbor : Graph[Current].Neighbors)
		{
			float TentativeGScore = GScore[Current] + 1;

			if (!GScore.Contains(Neighbor) || TentativeGScore < GScore[Neighbor])
			{
				CameFrom.Add(Neighbor, Current);
				GScore.Add(Neighbor, TentativeGScore);

				if (!OpenSet.Contains(Neighbor))
					OpenSet.Add(Neighbor);
			}
		}
	}

	// Aucun chemin trouvé
	UE_LOG(LogTemp, Warning, TEXT("Aucun chemin trouvé !"));
	return {};
}


UCheckPointComponent* GraphManager::GetNearestCheckpoint(FVector Location)
{
	UCheckPointComponent* NearestCheckPoint = nullptr;
	float MinDistance = FLT_MAX;

	// Parcours tous les CheckPointComponents du monde
	for (TObjectIterator<UCheckPointComponent> It; It; ++It)
	{
		UCheckPointComponent* CheckPoint = *It;

		if (CheckPoint && CheckPoint->GetOwner()) // Vérifie qu'il est valide
		{
			float Distance = FVector::Dist(Location, CheckPoint->GetComponentLocation());

			if (Distance < MinDistance)
			{
				MinDistance = Distance;
				NearestCheckPoint = CheckPoint;
			}
		}
	}

	return NearestCheckPoint;
}

UCheckPointComponent* GraphManager::GetRandomCheckpoint()
{
	if (Graph.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Graph is empty! No checkpoint available."));
		return nullptr;
	}

	// Convertir les clés de la map (les CheckPoints) en array
	TArray<UCheckPointComponent*> CheckPoints;
	Graph.GetKeys(CheckPoints);

	// Choisir un index aléatoire
	int32 RandomIndex = UKismetMathLibrary::RandomIntegerInRange(0, CheckPoints.Num() - 1);
	return CheckPoints[RandomIndex];
}
