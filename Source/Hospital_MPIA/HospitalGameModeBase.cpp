// Fill out your copyright notice in the Description page of Project Settings.


#include "HospitalGameModeBase.h"
#include "Math/UnrealMathUtility.h"
#include "Route/RouteStraight.h"

void AHospitalGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	if (!RouteStraight || !RouteTurn) {
		UE_LOG(LogTemp, Error, TEXT("1 acteur à spawn n'est pas assigné dans l'éditeur !"));
		return;
	}

	TArray<TSubclassOf<ARouteStraight>> RouteTypes;
	RouteTypes.Add(RouteStraight);
	RouteTypes.Add(RouteTurn);

	int16 Size = RouteTypes.Num();

	UWorld* World = GetWorld();
	if (World)
	{

		ARouteStraight* LastSpawnedActor = nullptr;
		float sizeX=0, sizeY=0;
		for (int i=0; i<3;++i)
		{
			for (int j = 0; j < 3; ++j) {
				// Position et rotation de spawn

				FVector SpawnLocation = FVector(i * sizeX, j * sizeY, 0);

				int32 RandomNumber = FMath::RandRange(0, Size - 1);

				// Spawner l'Actor
				ARouteStraight* SpawnedActor = createActor(World, RouteTypes[RandomNumber], SpawnLocation);

				FVector ActorSize = SpawnedActor->GetComponentsBoundingBox().GetSize();
				UE_LOG(LogTemp, Warning, TEXT("Taille du Blueprint : %s"), *ActorSize.ToString());
				sizeY = ActorSize.Y;
				sizeX = ActorSize.X;

				bool IsNotCorrect = true;
				int index = 0;
				while (IsNotCorrect && index < Size) {
					if (!LastSpawnedActor) {
						IsNotCorrect = false;
						break;
					}

					if (RouteIsCoherente(LastSpawnedActor, SpawnedActor))
					{
						IsNotCorrect = false;
						break;
					}

					UE_LOG(LogTemp, Warning, TEXT("Destruction de l'acteur (%d,%d)"), i,j);
					SpawnedActor->Destroy();

					// Spawner l'Actor
					SpawnedActor = createActor(World, RouteTypes[RandomNumber], SpawnLocation);

					ActorSize = SpawnedActor->GetComponentsBoundingBox().GetSize();
					UE_LOG(LogTemp, Warning, TEXT("Taille du Blueprint : %s"), *ActorSize.ToString());
					sizeY = ActorSize.Y;
					sizeX = ActorSize.X;

					index++;
					RandomNumber = (RandomNumber + 1) % Size;
				}


				LastSpawnedActor = SpawnedActor;
			}
		}
	}
	
}

ARouteStraight* AHospitalGameModeBase::createActor(UWorld* World, TSubclassOf<ARouteStraight> Actor, FVector SpawnLocation)
{
	ARouteStraight* SpawnedActor = World->SpawnActor<ARouteStraight>(Actor, SpawnLocation, FRotator(0, 0, 0));
	if (SpawnedActor)
	{
		UE_LOG(LogTemp, Warning, TEXT("Actor spawned successfully: %s"), *SpawnedActor->GetName());
	}

	return SpawnedActor;
}

bool AHospitalGameModeBase::RouteIsCoherente(ARouteStraight* LastActor, ARouteStraight* NewActor)
{
	if (LastActor->top && NewActor->down || LastActor->down && NewActor->top){
		return true;
	}
	if (LastActor->left && NewActor->right || LastActor->right && NewActor->left) {
		return true;
	}
	return false;
}
