// Fill out your copyright notice in the Description page of Project Settings.


#include "LowPolyFPS/Weapon/BaseWeapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimInstance.h"

// Sets default values
ABaseWeapon::ABaseWeapon()
{
    PrimaryActorTick.bCanEverTick = false;

    WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
    SetRootComponent(WeaponMesh);

    WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    WeaponMesh->SetOnlyOwnerSee(true); // Hide from others, visible to owner
    WeaponMesh->bCastDynamicShadow = false;
    WeaponMesh->CastShadow = false;
}

void ABaseWeapon::Fire()
{
    if (WeaponMesh && FireMontage)
    {
        UAnimInstance* AnimInstance = WeaponMesh->GetAnimInstance();
        if (AnimInstance)
        {
            AnimInstance->Montage_Play(FireMontage);
        }
    }
}

USkeletalMeshComponent* ABaseWeapon::GetWeaponMesh() const
{
    return WeaponMesh;
}