#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "Route/CheckPoint/CheckPointComponent.h"
#include "GraphManager.generated.h"

USTRUCT()
struct FGraphNode
{
    GENERATED_BODY()

    UPROPERTY()
    UCheckPointComponent* CheckPoint;

    UPROPERTY()
    TArray<UCheckPointComponent*> Neighbors;

    FGraphNode() : CheckPoint(nullptr) {}
    FGraphNode(UCheckPointComponent* InCheckPoint) : CheckPoint(InCheckPoint) {}
};

UCLASS()
class HOSPITAL_MPIA_API UGraphManager : public UWorldSubsystem
{
    GENERATED_BODY()

protected:

    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;

public:
    UGraphManager();

    void SetupGraph();

    TArray<UCheckPointComponent*> FindPath(UCheckPointComponent* Start, UCheckPointComponent* Goal);
    UCheckPointComponent* GetNearestCheckpoint(FVector Location);
    UCheckPointComponent* GetRandomCheckpoint();

    void AddCheckpoint(UCheckPointComponent* CheckPoint);

    void DrawGraphConnections();

private:
    TMap<UCheckPointComponent*, FGraphNode> Graph;
};
