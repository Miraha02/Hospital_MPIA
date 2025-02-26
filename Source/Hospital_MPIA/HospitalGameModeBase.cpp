// Fill out your copyright notice in the Description page of Project Settings.


#include "HospitalGameModeBase.h"
#include "Math/UnrealMathUtility.h"
#include "Route/RouteStraight.h"

void AHospitalGameModeBase::BeginPlay()
{
    Super::BeginPlay();

    if (!RouteStraight || !RouteTurn || !RouteT || !RouteCross) {
        UE_LOG(LogTemp, Error, TEXT("Un acteur à spawn n'est pas assigné dans l'éditeur !"));
        return;
    }

    TArray<TSubclassOf<ARouteStraight>> RouteTypes = {RouteStraight, RouteTurn, RouteT, RouteCross};
    int16 Size = RouteTypes.Num();

    UWorld* World = GetWorld();
    if (!World) return;

    TArray<TArray<ARouteStraight*>> SpawnedActors;
    SpawnedActors.SetNum(MapSize);
    for (auto& Row : SpawnedActors) Row.SetNum(MapSize);

    float sizeX = 0, sizeY = 0;

    for (int i = 0; i < MapSize; ++i)
    {
        for (int j = 0; j < MapSize; ++j)
        {
            UE_LOG(LogTemp, Warning, TEXT("\n\nSpawn route (%d,%d)"),i,j);
            FVector SpawnLocation = FVector(i * sizeX, j * sizeY, 0);
            ARouteStraight* SpawnedActor = nullptr;
            int32 RandomNumber = FMath::RandRange(0, Size - 1);

            bool IsNotCorrect = true;
            int rotationAttempts = 0;
            int pieceAttempts = 0;

            while (IsNotCorrect && pieceAttempts < Size)
            {
                // Spawn les coins de la map
                if (i==0 && j==0)
                {
                    SpawnedActor = createActor(World, RouteTypes[1], SpawnLocation);
                }
                else if (i==0 && j==MapSize - 1)
                {
                    SpawnedActor = createActor(World, RouteTypes[1], SpawnLocation);
                    RotateRoute(SpawnedActor);
                    RotateRoute(SpawnedActor);
                    RotateRoute(SpawnedActor);
                }
                else if (i==MapSize - 1 && j==0)
                {
                    SpawnedActor = createActor(World, RouteTypes[1], SpawnLocation);
                    RotateRoute(SpawnedActor);
                }
                else if (i==MapSize - 1 && j==MapSize - 1)
                {
                    SpawnedActor = createActor(World, RouteTypes[1], SpawnLocation);
                    RotateRoute(SpawnedActor);
                    RotateRoute(SpawnedActor);
                }

                //Spawn les arretes de la map
                else if (i==0)
                {
                    SpawnedActor = createActor(World, RouteTypes[2], SpawnLocation);
                }
                else if (i==MapSize - 1)
                {
                    SpawnedActor = createActor(World, RouteTypes[2], SpawnLocation);
                    RotateRoute(SpawnedActor);
                    RotateRoute(SpawnedActor);
                }
                else if (j==0)
                {
                    SpawnedActor = createActor(World, RouteTypes[2], SpawnLocation);
                    RotateRoute(SpawnedActor);
                }
                else if (j==MapSize - 1)
                {
                    SpawnedActor = createActor(World, RouteTypes[2], SpawnLocation);
                    RotateRoute(SpawnedActor);
                    RotateRoute(SpawnedActor);
                    RotateRoute(SpawnedActor);
                }
                // Spawn un actor à l'intérieur de la map
                else
                {
                    SpawnedActor = createActor(World, RouteTypes[RandomNumber], SpawnLocation);
                }
                FVector ActorSize = SpawnedActor->GetComponentsBoundingBox().GetSize();
                sizeX = ActorSize.X;
                sizeY = ActorSize.Y;

                rotationAttempts = 0;
                while (rotationAttempts < 4)
                {
                    if (IsPlacementValid(SpawnedActors, i, j, SpawnedActor))
                    {
                        IsNotCorrect = false;
                        break;
                    }
                    UE_LOG(LogTemp, Warning, TEXT("Rotation %d"),rotationAttempts+1);

                    RotateRoute(SpawnedActor);
                    
                    rotationAttempts++;
                }

                if (IsNotCorrect)
                {
                    SpawnedActor->Destroy();
                    pieceAttempts++;
                    RandomNumber = (RandomNumber + 1) % Size;
                }
            }

            if (SpawnedActor)
            {
                SpawnedActors[i][j] = SpawnedActor;
            }
        }
    }
}

bool AHospitalGameModeBase::IsPlacementValid(const TArray<TArray<ARouteStraight*>>& Grid, int x, int y, ARouteStraight* NewActor)
{
    if (y > 0 && Grid[x][y - 1] && !RouteIsCoherente(NewActor, Grid[x][y - 1], 0)) return false; // Haut (top)
    if (y < Grid[x].Num() - 1 && Grid[x][y + 1] && !RouteIsCoherente(NewActor, Grid[x][y + 1], 2)) return false; // Bas (down)
    if (x > 0 && Grid[x - 1][y] && !RouteIsCoherente(NewActor, Grid[x - 1][y], 3)) return false; // Gauche (left)
    if (x < Grid.Num() - 1 && Grid[x + 1][y] && !RouteIsCoherente(NewActor, Grid[x + 1][y], 1)) return false; // Droite (right)

    return true;
}


bool AHospitalGameModeBase::RouteIsCoherente(ARouteStraight* Neighbor, ARouteStraight* NewActor, int direction)
{
    switch (direction)
    {
    case 0: // Haut
        return (Neighbor->down && NewActor->top) || (!Neighbor->down && !NewActor->top);
    case 1: // Droite
        return (Neighbor->left && NewActor->right) || (!Neighbor->left && !NewActor->right);
    case 2: // Bas
        return (Neighbor->top && NewActor->down) || (!Neighbor->top && !NewActor->down);
    case 3: // Gauche
        return (Neighbor->right && NewActor->left) || (!Neighbor->right && !NewActor->left);
    default:
        return false;
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

void AHospitalGameModeBase::RotateRoute(ARouteStraight* Actor)
{
    Actor->AddActorLocalRotation(FRotator(0, 90, 0));
    bool tmp=Actor->top;
    Actor->top = Actor->left;
    Actor->left = Actor->down;
    Actor->down = Actor->right;
    Actor->right = tmp;
}
