// Fill out your copyright notice in the Description page of Project Settings.


#include "GraphManager.h"

#include "Kismet/GameplayStatics.h"

// Sets default values
AGraphManager::AGraphManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AGraphManager::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGraphManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGraphManager::SetupGraph()
{
	// Récupérer tous les CheckPoints dans la scène
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), FoundActors);

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

TArray<UCheckPointComponent*> AGraphManager::FindPath(UCheckPointComponent* Start, UCheckPointComponent* Goal)
{
	return TArray<UCheckPointComponent*>();
}

