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

    virtual void Fire();

    UFUNCTION(BlueprintCallable, Category = "Weapon")
    USkeletalMeshComponent* GetWeaponMesh() const;

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
    USkeletalMeshComponent* WeaponMesh;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
    UAnimMontage* FireMontage;
};
