#pragma once
#include "Hospital_MPIA/Character/MansionCharacter.h"
#include <Hospital_MPIA/Character/Controller/MansionAIController.h>

class Steering
{
public:

	Steering()
	{
		
	};

	FVector Seek(const AMansionCharacter* Character, FVector TargetLocation);

	FVector Arrival(const AMansionCharacter* Character, AMansionAIController* AIController, FVector TargetLocation);
	
};
