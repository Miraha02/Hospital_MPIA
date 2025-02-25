// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
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
	TSubclassOf<AActor> RouteStraight;
};
