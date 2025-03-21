// Fill out your copyright notice in the Description page of Project Settings.


#include "LowPolyFPS/Character/PlayerCharacter.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Controller.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Character.h"
#include "Components/TimelineComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    // Create Camera Component
    CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
    CameraComponent->SetupAttachment(RootComponent);
    CameraComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 64.0f));
    CameraComponent->bUsePawnControlRotation = true;

    // Timeline Component for Smooth Crouch
    CrouchTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("CrouchTimeline"));

    // Default Capsule Sizes
    OriginalCapsuleHalfHeight = GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight();
    CrouchCapsuleHalfHeight = OriginalCapsuleHalfHeight * 0.5f; // Half the size

    // Create First-Person Arms Mesh
    FPArmsMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FirstPersonArms"));
    FPArmsMesh->SetupAttachment(CameraComponent);
    FPArmsMesh->bCastDynamicShadow = false;
    FPArmsMesh->CastShadow = false;
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
    if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
    {
        if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
            ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
        {
            Subsystem->AddMappingContext(InputMappingContext, 0);
        }
    }

    if (CrouchCurve)
    {
        FOnTimelineFloat CrouchProgress;
        CrouchProgress.BindUFunction(this, FName("UpdateCrouch"));
        CrouchTimeline->AddInterpFloat(CrouchCurve, CrouchProgress);
    }
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

    if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
    {

        EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Move);

        EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Look);

        EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
        EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

        EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this, &APlayerCharacter::StartSprint);
        EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &APlayerCharacter::StopSprint);

        EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Started, this, &APlayerCharacter::StartCrouch);
        EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Completed, this, &APlayerCharacter::StopCrouch);
    }
}

void APlayerCharacter::Move(const FInputActionValue& Value)
{
    FVector2D MovementVector = Value.Get<FVector2D>();
    if (Controller != nullptr)
    {
        AddMovementInput(GetActorForwardVector(), MovementVector.Y);
        AddMovementInput(GetActorRightVector(), MovementVector.X);
    }
}

void APlayerCharacter::Look(const FInputActionValue& Value)
{
    FVector2D LookAxisVector = Value.Get<FVector2D>();
    if (Controller != nullptr)
    {
        AddControllerYawInput(LookAxisVector.X);
        AddControllerPitchInput(LookAxisVector.Y);
    }
}

// Sprint Functions
void APlayerCharacter::StartSprint()
{
    GetCharacterMovement()->MaxWalkSpeed = SprintSpeed; // Set sprint speed
}

void APlayerCharacter::StopSprint()
{
    GetCharacterMovement()->MaxWalkSpeed = WalkSpeed; // Reset to normal speed
}

void APlayerCharacter::StartCrouch()
{
    if (CrouchTimeline && CrouchCurve)
    {
        CrouchTimeline->PlayFromStart();
    }
}

void APlayerCharacter::StopCrouch()
{
    if (CrouchTimeline && CrouchCurve)
    {
        CrouchTimeline->Reverse();
    }
}

void APlayerCharacter::UpdateCrouch(float Value)
{
    float NewHeight = FMath::Lerp(OriginalCapsuleHalfHeight, CrouchCapsuleHalfHeight, Value);
    GetCapsuleComponent()->SetCapsuleHalfHeight(NewHeight);

    FVector MeshOffset = FVector(0, 0, NewHeight - OriginalCapsuleHalfHeight);
    CameraComponent->SetRelativeLocation(FVector(0, 0, 64.0f) + MeshOffset);
}
