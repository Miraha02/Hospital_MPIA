// Fill out your copyright notice in the Description page of Project Settings.


#include "MansionAIController.h"

#include "../MansionCharacter.h"

void AMansionAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	APawn* ControlledPawn = GetPawn();
	
	AMansionCharacter* MansionCharacter = Cast<AMansionCharacter>(ControlledPawn);
    
	if (MansionCharacter)
	{
		MansionCharacter->Move(FVector(1, 1, 0),1);
	}
}
