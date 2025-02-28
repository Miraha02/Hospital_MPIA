// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Hospital_MPIA/Data/HospitalDataAsset.h"
#include "MansionCharacter.generated.h"

UCLASS()
class HOSPITAL_MPIA_API AMansionCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Characters_Data")
	UHospitalDataAsset* HospitalDataAsset;
	
public:
	// Sets default values for this character's properties
	AMansionCharacter();

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UHospitalDataAsset* GetHospitalDataAsset() const;

	void Move(FVector ForwardDirection, float Movement);
	void Turn(float TurnValue);

};
