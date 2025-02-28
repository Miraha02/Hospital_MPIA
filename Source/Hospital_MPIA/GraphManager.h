#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Route/CheckPoint/CheckPointComponent.h"
#include "GraphManager.generated.h"

USTRUCT()
struct FGraphNode
{
	GENERATED_BODY()

	// Référence au CheckPoint associé
	UPROPERTY()
	UCheckPointComponent* CheckPoint;

	// Liste des voisins connectés
	UPROPERTY()
	TArray<UCheckPointComponent*> Neighbors;

	FGraphNode() : CheckPoint(nullptr) {}
	FGraphNode(UCheckPointComponent* InCheckPoint) : CheckPoint(InCheckPoint) {}
};

class HOSPITAL_MPIA_API GraphManager
{

public:

	GraphManager();

	
	// Map contenant tous les nœuds du graphe
	UPROPERTY()
	TMap<UCheckPointComponent*, FGraphNode> Graph;
	
	// Générer le graphe
	void SetupGraph();

	// Trouver le chemin entre deux CheckPoints
	TArray<UCheckPointComponent*> FindPath(UCheckPointComponent* Start, UCheckPointComponent* Goal);
	FVector GetNearestCheckpoint(FVector Location);
};
