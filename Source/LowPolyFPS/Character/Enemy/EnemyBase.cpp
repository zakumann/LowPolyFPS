// Fill out your copyright notice in the Description page of Project Settings.


#include "LowPolyFPS/Character/Enemy/EnemyBase.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "LowPolyFPS/Character/PlayerCharacter.h"

// Sets default values
AEnemyBase::AEnemyBase()
{
	PrimaryActorTick.bCanEverTick = false;
}