// Fill out your copyright notice in the Description page of Project Settings.


#include "MansionCharacter.h"

#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AMansionCharacter::AMansionCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMansionCharacter::BeginPlay()
{
	Super::BeginPlay();

	if(HospitalDataAsset)
	{
		GetCharacterMovement()->MaxWalkSpeed = HospitalDataAsset->Speed;
	}
}

// Called every frame
void AMansionCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMansionCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

UHospitalDataAsset* AMansionCharacter::GetHospitalDataAsset() const
{
	return HospitalDataAsset;
}