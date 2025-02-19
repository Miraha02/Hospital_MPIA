// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Route.h"
#include "RouteStraight.generated.h"

UCLASS()
class HOSPITAL_MPIA_API ARouteStraight : public AActor, public IRoute
{
	GENERATED_BODY()

private:
	
public:	
	// Sets default values for this actor's properties
	ARouteStraight();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
