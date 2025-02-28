// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "MansionAIController.generated.h"

/**
 * 
 */
UCLASS()
class HOSPITAL_MPIA_API AMansionAIController : public AAIController
{
	GENERATED_BODY()

protected:
	virtual void Tick(float DeltaTime) override;
};
