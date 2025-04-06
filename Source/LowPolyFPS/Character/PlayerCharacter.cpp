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

//Door
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "LowPolyFPS/Door/DoorActor.h"

//Weapon
#include "LowPolyFPS/Weapon/BaseWeapon.h"
#include <LowPolyFPS/Weapon/Pistol.h>

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
    FPArmsMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    FPArmsMesh->bRenderCustomDepth = true;
    FPArmsMesh->SetCustomDepthStencilValue(1); // Optional if using stencil mask
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

    // Spawn and attach pistol
    if (UWorld* World = GetWorld())
    {
        FActorSpawnParameters SpawnParams;
        SpawnParams.Owner = this;

        CurrentWeapon = World->SpawnActor<APistol>(APistol::StaticClass(), SpawnParams);
        if (CurrentWeapon)
        {
            CurrentWeapon->AttachToComponent(FPArmsMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("WeaponSocket")); // Make sure the socket exists
        }
    }

    SpawnStarterWeapon();
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
        // Fire
        EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Started, this, &APlayerCharacter::Fire);
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
}

void APlayerCharacter::Fire()
{
    if (CurrentWeapon)
    {
        CurrentWeapon->Fire();
    }
}

void APlayerCharacter::SpawnStarterWeapon()
{
    if (StarterWeaponClass)
    {
        FActorSpawnParameters SpawnParams;
        SpawnParams.Owner = this;
        CurrentWeapon = GetWorld()->SpawnActor<ABaseWeapon>(StarterWeaponClass, SpawnParams);

        if (CurrentWeapon)
        {
            CurrentWeapon->AttachToComponent(FPArmsMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("WeaponSocket"));
            CurrentWeapon->Fire();

            TArray<UPrimitiveComponent*> WeaponComponents;
            CurrentWeapon->GetComponents<UPrimitiveComponent>(WeaponComponents);
            for (auto* Comp : WeaponComponents)
            {
                Comp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
                Comp->bRenderCustomDepth = true;
                Comp->SetCustomDepthStencilValue(1);
            }
        }
    }
}
