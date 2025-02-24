// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include <EnhancedInputLibrary.h>
#include "MansionPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class HOSPITAL_MPIA_API AMansionPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
    // Références aux Inputs
    UPROPERTY(EditAnywhere, Category = "Input")
    UInputMappingContext* InputMapping;

    UPROPERTY(EditAnywhere, Category = "Input")
    UInputAction* IA_Move;

    UPROPERTY(EditAnywhere, Category = "Input")
    UInputAction* IA_Look;

    UPROPERTY(EditAnywhere, Category = "Input")
    UInputAction* IA_Turn;

protected:

    float Movement = 0.0f;

    virtual void BeginPlay() override;
    virtual void SetupInputComponent() override;

    void Move(const FInputActionValue& Value);
    void Look(const FInputActionValue& Value);
    void Turn(const FInputActionValue& Value);
};
