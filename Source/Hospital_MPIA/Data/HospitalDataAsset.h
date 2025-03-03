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
	
	UPROPERTY(EditAnywhere, Category="Movement")
	float Speed = 600;

	UPROPERTY(EditAnywhere, Category="Turn")
	float TurningFactor = 0.25;

	UPROPERTY(EditAnywhere, Category="Turn")
	float MinVelocityToTurn = 0.25;

	UPROPERTY(EditAnywhere, Category="Stop")
	float AcceptableStopDistance = 50.0;

	UPROPERTY(EditAnywhere, Category="Stop")
	float AcceptableStopSpeed = 10.0;

	UPROPERTY(EditAnywhere, Category = "Stop")
	float DistanceBeforeBreaking = 100.0;

	UPROPERTY(EditAnywhere, Category = "Stop")
	float BreakingFactor = 3;

	UPROPERTY(EditAnywhere, Category="DEBUG")
	bool ShowLog = true;
};
