// Fill out your copyright notice in the Description page of Project Settings.


#include "MansionPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include <GameFramework/SpringArmComponent.h>

void AMansionPlayerController::BeginPlay() {
    
    Super::BeginPlay();

    UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
    if (Subsystem)
    {
        Subsystem->AddMappingContext(InputMapping, 0);
    }
}


void AMansionPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();

    UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(InputComponent);
    if (EnhancedInput)
    {
        EnhancedInput->BindAction(IA_Move, ETriggerEvent::Triggered, this, &AMansionPlayerController::Move);
        EnhancedInput->BindAction(IA_Look, ETriggerEvent::Triggered, this, &AMansionPlayerController::Look);
        EnhancedInput->BindAction(IA_Turn, ETriggerEvent::Triggered, this, &AMansionPlayerController::Turn);
    }
}

void AMansionPlayerController::Move(const FInputActionValue& Value)
{
    UE_LOG(LogTemp, Warning, TEXT("Move Func !"));
    float Movement = Value.Get<float>();
    APawn* ControlledPawn = GetPawn();
    if (ControlledPawn)
    {
        // Récupérer la rotation actuelle du véhicule
        FRotator VehicleRotation = ControlledPawn->GetActorRotation();

        // Calculer les vecteurs de direction en fonction de l'orientation du véhicule
        FVector ForwardDirection = FRotationMatrix(VehicleRotation).GetUnitAxis(EAxis::X);

        // Ajouter le mouvement
        ControlledPawn->AddMovementInput(ForwardDirection, Movement);
    }
}

void AMansionPlayerController::Look(const FInputActionValue& Value)
{
    UE_LOG(LogTemp, Warning, TEXT("Look Func !"));
    FVector2D LookAxis = Value.Get<FVector2D>();

    APawn* ControlledPawn = GetPawn();
    if (ControlledPawn)
    {
        // Récupérer le SpringArm pour modifier la rotation indépendamment du véhicule
        USpringArmComponent* SpringArm = ControlledPawn->FindComponentByClass<USpringArmComponent>();
        if (SpringArm)
        {
            // Modifier la rotation de la caméra uniquement
            FRotator NewRotation = SpringArm->GetRelativeRotation();
            NewRotation.Yaw += LookAxis.X;
            NewRotation.Pitch = FMath::Clamp(NewRotation.Pitch + LookAxis.Y, -80.f, 80.f); // Limite du pitch

            SpringArm->SetRelativeRotation(NewRotation);
        }
    }
}

void AMansionPlayerController::Turn(const FInputActionValue& Value)
{
    float TurnValue = Value.Get<float>();
    APawn* ControlledPawn = GetPawn();

    if (ControlledPawn && Movement != 0) // Vérifie si on avance
    {
        // Récupérer la rotation actuelle du véhicule
        FRotator VehicleRotation = ControlledPawn->GetActorRotation();

        // Modifier seulement l'angle de Yaw pour tourner
        VehicleRotation.Yaw += TurnValue;

        // Appliquer la nouvelle rotation
        ControlledPawn->SetActorRotation(VehicleRotation);
    }
}