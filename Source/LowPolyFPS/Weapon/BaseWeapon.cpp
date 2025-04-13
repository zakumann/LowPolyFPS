// Fill out your copyright notice in the Description page of Project Settings.


#include "LowPolyFPS/Weapon/BaseWeapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimInstance.h"

// Sets default values
ABaseWeapon::ABaseWeapon()
{
    WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));

    WeaponName = "Default";
    WeaponMaxAmmo = 255;
    MagazineMaxAmmo = 30;

    CurrentTotalAmmo = WeaponMaxAmmo;
    CurrentMagazineAmmo = MagazineMaxAmmo;

    WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    WeaponMesh->SetOnlyOwnerSee(false);
    WeaponMesh->bCastDynamicShadow = false;
    WeaponMesh->CastShadow = false;
    WeaponMesh->SetVisibility(true);
    WeaponMesh->SetHiddenInGame(false);
}

void ABaseWeapon::Fire()
{
}

void ABaseWeapon::Reload()
{
}

TArray<int> ABaseWeapon::GetCurrentAmmo()
{
    return { CurrentMagazineAmmo, CurrentTotalAmmo };
}

