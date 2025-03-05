// Fill out your copyright notice in the Description page of Project Settings.


#include "GraphManager.h"

#include "AITestsCommon.h"
#include "AITestsCommon.h"
#include "AITestsCommon.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

void UGraphManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	Graph.Empty();
}

void UGraphManager::Deinitialize()
{
	Super::Deinitialize();
	Graph.Empty();
}

// Sets default values
UGraphManager::UGraphManager()
{

}

void UGraphManager::SetupGraph()
{
	UE_LOG(LogTemp, Warning, TEXT("Setting up the Connections graph"));

	// S'assurer que le graphe contient bien des checkpoints
	if (Graph.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("No checkpoints found in the graph."));
		return;
	}

	// Remplir les connexions entre les noeuds
	for (auto& Entry : Graph)
	{
		UCheckPointComponent* CheckPoint = Entry.Key;
		FGraphNode& Node = Entry.Value;

		if (CheckPoint)
		{
			CheckPoint->DetectNeighbors();
			// Ajouter les connexions aux voisins
			for (UCheckPointComponent* Neighbor : CheckPoint->ConnectedCheckPoints)
			{
				if (Graph.Contains(Neighbor))
				{
					Node.Neighbors.Add(Neighbor);
					UE_LOG(LogTemp, Warning, TEXT("Connected %s to %s"),
						*CheckPoint->GetName(), *Neighbor->GetName());
				}
			}
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("Graph setup complete with %d nodes"), Graph.Num());
}



TArray<UCheckPointComponent*> UGraphManager::FindPath(UCheckPointComponent* Start, UCheckPointComponent* Goal)
{
	if (!Graph.Contains(Start) || !Graph.Contains(Goal))
	{
		UE_LOG(LogTemp, Warning, TEXT("Au moins l'un des composants cible n'est pas présents dans le graphe !"))
		return {};
	}

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


UCheckPointComponent* UGraphManager::GetNearestCheckpoint(FVector Location)
{
	UCheckPointComponent* NearestCheckPoint = nullptr;
	float MinDistance = FLT_MAX;


	for (auto& Entry : Graph)
	{
		UCheckPointComponent* CheckPoint = Entry.Key;

		if (CheckPoint)
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

UCheckPointComponent* UGraphManager::GetRandomCheckpoint()
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

void UGraphManager::AddCheckpoint(UCheckPointComponent* CheckPoint)
{
	if (!CheckPoint)
		return;

	if (!Graph.Contains(CheckPoint))
	{
		Graph.Add(CheckPoint, FGraphNode(CheckPoint));
		UE_LOG(LogTemp, Warning, TEXT("Checkpoint ajouté au graphe: %s"), *CheckPoint->GetName());
	}
}

void UGraphManager::DrawGraphConnections()
{
	if (!GetWorld()) return;

	for (const auto& Entry : Graph)
	{
		UCheckPointComponent* CheckPoint = Entry.Key;
		const FGraphNode& Node = Entry.Value;

		if (CheckPoint)
		{
			FVector StartLocation = CheckPoint->GetComponentLocation();

			for (UCheckPointComponent* ConnectedPoint : Node.Neighbors)
			{
				if (ConnectedPoint)
				{
					FVector EndLocation = ConnectedPoint->GetComponentLocation();

					// Tracer une ligne entre les deux CheckPoints
					DrawDebugLine(GetWorld(),
						StartLocation,
						EndLocation,
						FColor::Blue,
						false,  // Persiste seulement temporairement
						15.0f,   // Durée d'affichage
						0,      // Aucun canal de profondeur
						5.f     // Épaisseur de la ligne
					);
				}
			}
		}
	}
}
