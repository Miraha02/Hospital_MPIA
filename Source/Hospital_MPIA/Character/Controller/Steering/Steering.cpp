#include "Steering.h"


FVector Steering::Seek(const AMansionCharacter* Character, FVector TargetLocation, FVector ActorLocation)
{
	FVector desired_velocity = TargetLocation - ActorLocation;

	FVector Velocity = Character->GetVelocity();

	UHospitalDataAsset* HospitalDataAsset = Character->HospitalDataAsset;
	if (!HospitalDataAsset)
	{
		UE_LOG(LogTemp, Error, TEXT("HospitalDataAsset is Not Assigned in BP Details"));
		return FVector::ZeroVector;
	}
	desired_velocity = desired_velocity.GetSafeNormal() * HospitalDataAsset->Speed;
	FVector steering = desired_velocity - Velocity;

	return steering;
}
