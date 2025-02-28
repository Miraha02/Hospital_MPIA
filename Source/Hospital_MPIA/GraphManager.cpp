// Fill out your copyright notice in the Description page of Project Settings.


#include "GraphManager.h"

#include "AITestsCommon.h"
#include "AITestsCommon.h"
#include "AITestsCommon.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
GraphManager::GraphManager()
{

}

void GraphManager::SetupGraph()
{
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

	// Priority queue (Min-Heap)
	TMap<UCheckPointComponent*, float> GScore;
	TMap<UCheckPointComponent*, float> FScore;
	TMap<UCheckPointComponent*, UCheckPointComponent*> CameFrom;

	TArray<UCheckPointComponent*> OpenSet;
	OpenSet.Add(Start);
	GScore.Add(Start, 0);
	FScore.Add(Start, FVector::Dist(Start->GetComponentLocation(), Goal->GetComponentLocation()));

	while (OpenSet.Num() > 0)
	{
		// Trouver le noeud avec le plus petit FScore
		UCheckPointComponent* Current = OpenSet[0];
		for (UCheckPointComponent* Node : OpenSet)
		{
			if (FScore.Contains(Node) && FScore[Node] < FScore[Current])
				Current = Node;
		}

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

		OpenSet.Remove(Current);

		// Parcourir les voisins
		for (UCheckPointComponent* Neighbor : Graph[Current].Neighbors)
		{
			float TentativeGScore = GScore[Current] + FVector::Dist(Current->GetComponentLocation(), Neighbor->GetComponentLocation());

			if (!GScore.Contains(Neighbor) || TentativeGScore < GScore[Neighbor])
			{
				CameFrom.Add(Neighbor, Current);
				GScore.Add(Neighbor, TentativeGScore);
				FScore.Add(Neighbor, TentativeGScore + FVector::Dist(Neighbor->GetComponentLocation(), Goal->GetComponentLocation()));

				if (!OpenSet.Contains(Neighbor))
					OpenSet.Add(Neighbor);
			}
		}
	}

	// Aucun chemin trouvé
	return {};
}

FVector GraphManager::GetNearestCheckpoint(FVector Location)
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

	return NearestCheckPoint->GetComponentLocation();
}
