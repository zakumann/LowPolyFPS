// Fill out your copyright notice in the Description page of Project Settings.


#include "LowPolyFPS/Weapon/Pistol.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraComponent.h"

APistol::APistol()
{
    PrimaryActorTick.bCanEverTick = false;
}

void APistol::Fire()
{
    Super::Fire();

    // Add muzzle flash, fire sound, trace logic here
    UE_LOG(LogTemp, Log, TEXT("Pistol fired!"));
}
