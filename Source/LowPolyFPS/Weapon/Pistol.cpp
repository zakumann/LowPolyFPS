// Fill out your copyright notice in the Description page of Project Settings.


#include "LowPolyFPS/Weapon/Pistol.h"

APistol::APistol()
{
	WeaponName = "1911";
	WeaponMaxAmmo = 98;
	MagazineMaxAmmo = 7;

	CurrentTotalAmmo = WeaponMaxAmmo;
	CurrentMagazineAmmo = MagazineMaxAmmo;
}

void APistol::Fire()
{
	UE_LOG(LogTemp, Warning, TEXT("Shooting 1911"));
}

void APistol::Reload()
{
}