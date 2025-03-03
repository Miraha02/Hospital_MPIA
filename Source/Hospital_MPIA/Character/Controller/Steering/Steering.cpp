#include "Steering.h"

#include "Chaos/PBDSuspensionConstraintData.h"


FVector Steering::Seek(const AMansionCharacter* Character, FVector TargetLocation)
{
	FVector ActorLocation = Character->GetActorLocation();
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

FVector Steering::Arrival(const AMansionCharacter* Character, AMansionAIController* AIController, FVector TargetLocation)
{
	UHospitalDataAsset* HospitalDataAsset = Character->HospitalDataAsset;
	FVector ActorLocation = Character->GetActorLocation();

	if (!HospitalDataAsset)
	{
		UE_LOG(LogTemp, Error, TEXT("HospitalDataAsset is Not Assigned in BP Details"));
		return FVector::ZeroVector;
	}
	
	if (AIController->TargetReached)
	{
		if (HospitalDataAsset->ShowLog)
			UE_LOG(LogTemp, Warning, TEXT("Target has been reached !"));
		return FVector(0,0,0);
	}
	
	float MaxSpeed = HospitalDataAsset->Speed;
	FVector Velocity = Character->GetVelocity();
	
	FVector desired_velocity = TargetLocation - ActorLocation;
	float distance = desired_velocity.Length();
	float ramped_speed = MaxSpeed * (distance / HospitalDataAsset->DistanceBeforeBreaking) * (1 / HospitalDataAsset->BreakingFactor);
	float clipped_speed = std::min(ramped_speed, MaxSpeed);

	desired_velocity = desired_velocity.GetSafeNormal() * clipped_speed;
	
	FVector steering = desired_velocity - Velocity;

	// Condition d'arrêt si l'acteur est proche de la cible et sa vitesse est faible
	if ((TargetLocation - ActorLocation).Size() <= HospitalDataAsset->AcceptableStopDistance && Velocity.Size() < HospitalDataAsset->AcceptableStopSpeed)
	{
		steering = FVector::ZeroVector;  // Arrêter l'acteur
		AIController->TargetReached = true;
	}

	return steering;
}
