// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseWeapon.generated.h"

UCLASS()
class LOWPOLYFPS_API ABaseWeapon : public AActor
{
    GENERATED_BODY()

public:
    ABaseWeapon();

protected:
    UPROPERTY(VisibleAnywhere, Category = "Weapon")
    class USkeletalMeshComponent* WeaponMesh;

    UPROPERTY(EditAnywhere, Category = "Weapon")
    class UAnimationAsset* FireAnimation;

    UPROPERTY(EditAnywhere, Category = "Weapon")
    class UAnimationAsset* ReloadAnimation;

    UPROPERTY(EditAnywhere, Category = "Weapon")
    FString WeaponName;

    UPROPERTY(EditAnywhere, Category = "Weapon")
    int32 WeaponMaxAmmo;

    UPROPERTY(EditAnywhere, Category = "Weapon")
    int32 MagazineMaxAmmo;

    int32 CurrentTotalAmmo;
    int32 CurrentMagazineAmmo;

public:
    virtual void Fire(); 

    virtual void Reload();

    //First element is Magazine Ammo, second element is Total Ammo
    TArray<int> GetCurrentAmmo();
};
