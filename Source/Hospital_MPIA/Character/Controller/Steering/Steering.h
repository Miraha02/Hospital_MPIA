#pragma once
#include "Hospital_MPIA/Character/MansionCharacter.h"

class Steering
{
public:

	Steering()
	{
		
	};

	FVector Seek(const AMansionCharacter* Character, FVector TargetLocation, FVector ActorLocation);
	
};
