// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Route/RouteStraight.h"
#include "HospitalGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class HOSPITAL_MPIA_API AHospitalGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

	virtual void BeginPlay() override;

public:
	UPROPERTY(EditDefaultsOnly, Category = "MapObject")
	TSubclassOf<ARouteStraight> RouteStraight;

	UPROPERTY(EditDefaultsOnly, Category = "MapObject")
	TSubclassOf<ARouteStraight> RouteTurn;

	UPROPERTY(EditDefaultsOnly, Category = "MapObject")
	TSubclassOf<ARouteStraight> RouteT;

	UPROPERTY(EditDefaultsOnly, Category = "MapObject")
	TSubclassOf<ARouteStraight> RouteCross;

	UPROPERTY(EditDefaultsOnly, Category = "MapObject")
	int MapSize = 3;

private:
	ARouteStraight* createActor(UWorld* World, TSubclassOf<ARouteStraight> Actor, FVector SpawnLocation);

	bool IsPlacementValid(const TArray<TArray<ARouteStraight*>>& Grid, int x, int y, ARouteStraight* NewActor);
	bool RouteIsCoherente(ARouteStraight* Neighbor, ARouteStraight* NewActor, int direction);
};
