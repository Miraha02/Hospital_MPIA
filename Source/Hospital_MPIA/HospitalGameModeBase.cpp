// Fill out your copyright notice in the Description page of Project Settings.


#include "HospitalGameModeBase.h"

#include "Route/RouteStraight.h"

void AHospitalGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	if (RouteStraight) // Vérifier si la classe est assignée
	{
		UWorld* World = GetWorld();
		if (World)
		{

			float sizeX=0, sizeY=0;
			for (int i=0; i<3;++i)
			{
				// Position et rotation de spawn
				FVector SpawnLocation = FVector(i*sizeX, i*sizeY, 0);

				// Spawner l'Actor
				ARouteStraight* SpawnedActor = World->SpawnActor<ARouteStraight>(RouteStraight, SpawnLocation, FRotator(0, 0, 0));
				if (SpawnedActor)
				{
					UE_LOG(LogTemp, Warning, TEXT("Actor spawned successfully: %s"), *SpawnedActor->GetName());
					FVector ActorSize = SpawnedActor->GetComponentsBoundingBox().GetSize();
					UE_LOG(LogTemp, Warning, TEXT("Taille du Blueprint : %s"), *ActorSize.ToString());
					sizeY = ActorSize.Y;
				}
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ActorToSpawnClass n'est pas assigné dans l'éditeur !"));
	}
}
