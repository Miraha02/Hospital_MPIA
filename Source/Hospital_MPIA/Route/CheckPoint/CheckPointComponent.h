#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "CheckPointComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent), Blueprintable)
class HOSPITAL_MPIA_API UCheckPointComponent : public UStaticMeshComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCheckPointComponent();

	// Liste des connexions à d'autres CheckPoints
	UPROPERTY(BlueprintReadOnly, Category = "Connections")
	TArray<UCheckPointComponent*> ConnectedCheckPoints;

protected:
	void DetectNeighbors();
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Ajoute un CheckPoint à la liste des connexions
	UFUNCTION(BlueprintCallable, Category = "Connections")
	void ConnectTo(UCheckPointComponent* OtherCheckPoint);

	// Vérifie si un CheckPoint est déjà connecté
	UFUNCTION(BlueprintCallable, Category = "Connections")
	bool IsConnectedTo(UCheckPointComponent* OtherCheckPoint) const;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
