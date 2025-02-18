// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CheckPoint/CheckPointComponent.h"
#include "Route.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class URoute : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class HOSPITAL_MPIA_API IRoute
{
	GENERATED_BODY()

	virtual TArray<UCheckPointComponent*> GetCheckPointComponents() const = 0;
	virtual TArray<FVector> GetCheckPointLocation() const = 0;
public:
};
