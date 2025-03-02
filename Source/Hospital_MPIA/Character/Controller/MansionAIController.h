// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Hospital_MPIA/GraphManager.h"
#include "MansionAIController.generated.h"

/**
 * 
 */
UCLASS()
class HOSPITAL_MPIA_API AMansionAIController : public AAIController
{
	GENERATED_BODY()

private:
	
	FVector TargetLocation;
	
protected:

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	void ChooseRandomCheckpoint();

public:
	
	GraphManager* GraphManager;
};
