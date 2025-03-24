// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

struct FInputActionValue;

UCLASS()
class LOWPOLYFPS_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

    // Called every frame
    virtual void Tick(float DeltaTime) override;

    // Called to bind functionality to input
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    // First-Person Camera
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
    class UCameraComponent* FirstPersonCamera;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

    // Camera Component
    UPROPERTY(VisibleAnywhere, Category = "Camera")
    class UCameraComponent* CameraComponent;

    // First-Person Arms Mesh
    UPROPERTY(VisibleAnywhere, Category = "Mesh")
    class USkeletalMeshComponent* FPArmsMesh;

    // Input Actions
    UPROPERTY(EditAnywhere, Category = "Input")
    class UInputMappingContext* InputMappingContext;

    UPROPERTY(EditAnywhere, Category = "Input")
    class UInputAction* MoveAction;

    UPROPERTY(EditAnywhere, Category = "Input")
    class UInputAction* LookAction;

    UPROPERTY(EditAnywhere, Category = "Input")
    class UInputAction* JumpAction;

    UPROPERTY(EditAnywhere, Category = "Input")
    class UInputAction* SprintAction;

    UPROPERTY(EditAnywhere, Category = "Input")
    class UInputAction* CrouchAction;

    // Movement Functions
    void Move(const FInputActionValue& Value);
    void Look(const FInputActionValue& Value);

    // Sprint and Jump Functions
    void StartSprint();
    void StopSprint();

    void StartCrouch();
    void StopCrouch();

    /** Movement Speed */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float WalkSpeed = 600.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float SprintSpeed = 1200.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float CrouchSpeed= 450.0f;

    UPROPERTY(EditAnywhere, Category = "Crouch")
    float Crouching = 0.2f;

    // Timeline for smooth crouching
    UPROPERTY()
    class UTimelineComponent* CrouchTimeline;

    // Curve asset for timeline interpolation
    UPROPERTY(EditAnywhere, Category = "Crouch")
    UCurveFloat* CrouchCurve;

    // Crouch & Stand Heights
    UPROPERTY(EditAnywhere, Category = "Crouch")
    float CrouchHeight = 40.0f;

    UPROPERTY(EditAnywhere, Category = "Crouch")
    float StandHeight = 88.0f;

    UFUNCTION()
    void UpdateCrouch(float Value);

    UPROPERTY(EditAnywhere, Category = Gameplay)
    bool isCrouching;
};
