// Fill out your copyright notice in the Description page of Project Settings.


#include "LowPolyFPS/Weapon/Pistol.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraComponent.h"
#include "DrawDebugHelpers.h"
#include <LowPolyFPS/Character/PlayerCharacter.h>

APistol::APistol()
{
    PrimaryActorTick.bCanEverTick = false;
}

void APistol::Fire()
{
    Super::Fire();

    UE_LOG(LogTemp, Log, TEXT("Pistol fired!"));

    if (!WeaponMesh || !WeaponMesh->DoesSocketExist("Muzzle"))
    {
        UE_LOG(LogTemp, Warning, TEXT("No valid muzzle socket on weapon mesh."));
        return;
    }

    APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    if (!PC) return;

    FVector WorldLocation, WorldDirection;

    // Deproject the center of the screen (crosshair position) to world
    int32 ViewportX, ViewportY;
    PC->GetViewportSize(ViewportX, ViewportY);
    FVector2D ScreenCenter(ViewportX * 0.5f, ViewportY * 0.5f);

    if (!PC->DeprojectScreenPositionToWorld(ScreenCenter.X, ScreenCenter.Y, WorldLocation, WorldDirection))
    {
        UE_LOG(LogTemp, Warning, TEXT("Failed to deproject screen position to world."));
        return;
    }

    // Get the start location from muzzle
    FVector MuzzleLocation = WeaponMesh->GetSocketLocation("Muzzle");

    // Set end point in aiming direction (e.g., 10,000 units forward)
    FVector TraceEnd = MuzzleLocation + (WorldDirection * 10000.f);

    FHitResult Hit;
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(this);

    APlayerCharacter* Player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
    if (Player)
    {
        Params.AddIgnoredActor(Player);
    }

    bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, MuzzleLocation, TraceEnd, ECC_Visibility, Params);

    if (bHit && Hit.GetActor())
    {
        UE_LOG(LogTemp, Warning, TEXT("Raycast hit: %s"), *Hit.GetActor()->GetName());

        UGameplayStatics::ApplyPointDamage(Hit.GetActor(), 10.f, WorldDirection, Hit, PC, this, nullptr);
    }

    // Debug line
    DrawDebugLine(GetWorld(), MuzzleLocation, TraceEnd, bHit ? FColor::Red : FColor::Green, false, 1.0f, 0, 1.0f);
}
