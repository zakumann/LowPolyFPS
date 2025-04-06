// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LowPolyFPS/Weapon/BaseWeapon.h"
#include "Pistol.generated.h"

/**
 * 
 */
UCLASS()
class LOWPOLYFPS_API APistol : public ABaseWeapon
{
	GENERATED_BODY()
	
public:
	APistol();

	virtual void Fire() override;
};