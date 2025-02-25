// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "HospitalDataAsset.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class HOSPITAL_MPIA_API UHospitalDataAsset : public UDataAsset
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(EditAnywhere, Category="Turn")
	float TurningFactor = 0.25;
};
