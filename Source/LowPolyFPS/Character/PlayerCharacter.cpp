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
#include "GameFramework/PlayerController.h"
#include "Components/BoxComponent.h"

//Door
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "LowPolyFPS/Door/DoorActor.h"

//Weapon
#include "LowPolyFPS/Weapon/BaseWeapon.h"

//Ladder
#include "LowPolyFPS/Ladder.h"

//Enemy
#include "LowPolyFPS/Character/Enemy/EnemyBase.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

    // Create Camera Component
    FirstPersonCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
    FirstPersonCamera->SetupAttachment(GetCapsuleComponent());
    FirstPersonCamera->SetRelativeLocation(FVector(-29.0f, 0.0f, 44.0f));
    FirstPersonCamera->bUsePawnControlRotation = true;

    // Create First-Person Arms Mesh
    FPArmsMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FirstPersonArms"));
    FPArmsMesh->SetupAttachment(FirstPersonCamera);
    FPArmsMesh->bCastDynamicShadow = false;
    FPArmsMesh->CastShadow = false;

    WeaponIndex = 0;
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

    // Spawn Weapon using StartingWeaponClass
    if (ABaseWeapon* Weapon = GetWorld()->SpawnActor<ABaseWeapon>(StartingWeaponClass))
    {
        Weapon->AttachToComponent(FPArmsMesh, FAttachmentTransformRules::SnapToTargetIncludingScale, FName("WeaponSocket"));
    }
    // Attach spawned weapon to WeaponSocket
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

        // Interact
        EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &APlayerCharacter::Interact);
        // Climb
        EnhancedInputComponent->BindAction(ClimbUpDownAction, ETriggerEvent::Triggered, this, &APlayerCharacter::ClimbLadder);

        // Fire
        EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Started, this, &APlayerCharacter::Fire);
    }
}

void APlayerCharacter::Move(const FInputActionValue& Value)
{
    FVector2D MovementVector = Value.Get<FVector2D>();
    if (!Controller) return;

    if (!bIsOnLadder)
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
    GetCharacterMovement()->MaxWalkSpeed = CrouchSpeed;
    ACharacter::Crouch();
    isCrouching = true;
}

void APlayerCharacter::StopCrouch()
{
    GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
    ACharacter::UnCrouch();
    isCrouching = false;
}

void APlayerCharacter::Interact()
{
    // raycast range
    FHitResult HitResult;
    FVector Start = FirstPersonCamera->GetComponentLocation();
    FVector End = Start + FirstPersonCamera->GetForwardVector() * InteractLineTraceLength;
    GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECollisionChannel::ECC_Visibility);

    ADoorActor* Door = Cast<ADoorActor>(HitResult.GetActor());
    if (Door)
    {
        Door->PlayerCharacter = this;
        Door->OnInteract();
    }
    //Ladder status
    if (CurrentLadder)
    {
        // Toggle climbing state
        if (!bIsOnLadder)
        {
            bIsOnLadder = true;
            GetCharacterMovement()->SetMovementMode(MOVE_Flying);
        }
        else
        {
            bIsOnLadder = false;
            GetCharacterMovement()->SetMovementMode(MOVE_Walking);
        }
    }
}

void APlayerCharacter::ClimbLadder(const FInputActionValue& Value)
{
    if (bIsOnLadder && CurrentLadder && CurrentLadder->LadderTrigger && CurrentLadder->LadderTrigger->IsOverlappingActor(this))
    {
        float Direction = Value.Get<float>();
        AddMovementInput(FVector::UpVector, Direction);
    }
}

void APlayerCharacter::Fire()
{
    FVector FireStart = FirstPersonCamera->GetComponentLocation();
    FVector FireRot = FirstPersonCamera->GetComponentRotation().Vector();
    FVector FireEnd = FireStart + FireRot * 2000.0f;

    TArray<FHitResult> HitResults;
    FCollisionQueryParams CollisionParams;
    FCollisionResponseParams CollisionResponse;
    CollisionParams.AddIgnoredActor(this);

    if (GetWorld()->LineTraceMultiByChannel(OUT HitResults, FireStart, FireEnd, ECollisionChannel::ECC_Visibility, CollisionParams, CollisionResponse))
    {
        for (FHitResult& Result : HitResults)
        {
            if (AActor* HitActor = Result.GetActor())
            {
                UE_LOG(LogTemp, Warning, TEXT("Actor Name: %s"), *HitActor->GetName());
            }
        }

        //if (AEnemyBase* Enemy = Cast<AEnemyBase>(HitResult.GetActor()))
        //{
        //    UE_LOG(LogTemp, Warning, TEXT("Enemy HIT %s"), *Enemy->GetName());
        //    //Enemy->Hit(this);
        //}
    }

    DrawDebugLine(GetWorld(), FireStart, FireEnd, FColor::Red, false, 2.0f, 0, 3.0f);
}

void APlayerCharacter::Landed(const FHitResult& Hit)
{
    Super::Landed(Hit);

    // Exit ladder mode and switch to walking
    if (bIsOnLadder)
    {
        bIsOnLadder = false;
        GetCharacterMovement()->SetMovementMode(MOVE_Walking);
    }
}
