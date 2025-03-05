#include "CheckPointComponent.h"
#include "../../GraphManager.h"

// Constructeur
UCheckPointComponent::UCheckPointComponent()
{
	PrimaryComponentTick.bCanEverTick = true; // Pas besoin de Tick en général
}

// Called when the game starts
void UCheckPointComponent::BeginPlay()
{
	Super::BeginPlay();

	UGraphManager* GraphManager = GetWorld()->GetSubsystem<UGraphManager>();
	if (GraphManager)
	{
		GraphManager->AddCheckpoint(this);
	}

	/*
	// Attendre 0.1s avant d'effectuer le SphereTrace pour laisser le temps aux rotations d'être appliquées
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UCheckPointComponent::DetectNeighbors, 0.1f, false);
	*/
}


void UCheckPointComponent::DetectNeighbors()
{
	if (!GetWorld()) return;

	TArray<FHitResult> HitResults;
	FVector Center = GetComponentLocation();
	float DetectionRadius = 150.f; // Augmenté pour s'assurer qu'on capte bien les voisins

	// Debug : Voir la sphère en jeu
	DrawDebugSphere(GetWorld(), Center, DetectionRadius, 12, FColor::Green, false, 25.f);

	FCollisionShape Sphere = FCollisionShape::MakeSphere(DetectionRadius);
    
	// Correction : Faire un léger déplacement pour éviter un "Sweep statique"
	FVector EndLocation = Center + FVector(1, 1, 0);

	bool bHit = GetWorld()->SweepMultiByChannel(
		HitResults,
		Center,
		EndLocation, // Légère différence avec Start
		FQuat::Identity,
		ECC_WorldDynamic, // Changer le canal de collision
		Sphere
	);

	if (bHit)
	{
		for (const FHitResult& Hit : HitResults)
		{
			UCheckPointComponent* OtherCheckPoint = Cast<UCheckPointComponent>(Hit.GetComponent());
			if (OtherCheckPoint && OtherCheckPoint != this && !IsConnectedTo(OtherCheckPoint))
			{
				ConnectTo(OtherCheckPoint);
			}
		}
	}
}

// Ajouter une connexion entre deux CheckPoints
void UCheckPointComponent::ConnectTo(UCheckPointComponent* OtherCheckPoint)
{
	if (OtherCheckPoint && OtherCheckPoint != this && !ConnectedCheckPoints.Contains(OtherCheckPoint))
	{
		ConnectedCheckPoints.Add(OtherCheckPoint);
		OtherCheckPoint->ConnectedCheckPoints.Add(this); // Connexion bidirectionnelle
	}
}

// Vérifier si un CheckPoint est connecté à un autre
bool UCheckPointComponent::IsConnectedTo(UCheckPointComponent* OtherCheckPoint) const
{
	return ConnectedCheckPoints.Contains(OtherCheckPoint);
}

void UCheckPointComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}