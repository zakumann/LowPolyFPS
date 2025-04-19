// Fill out your copyright notice in the Description page of Project Settings.


#include "LowPolyFPS/Character/Enemy/EnemyBase.h"
#include "LowPolyFPS/Character/Enemy/EnemyFSM.h"

// Sets default values
AEnemyBase::AEnemyBase()
{
	PrimaryActorTick.bCanEverTick = true;

	// 1. Skeletal Mesh Data load...
	ConstructorHelpers::FObjectFinder<USkeletalMesh> tempMesh(TEXT("/Game/PolygonPrototype/Meshes/Characters/SK_Character_Male_Face_01.SK_Character_Male_Face_01"));
	// 1-1. if data load success...
	if (tempMesh.Succeeded())
	{
		//1-2 data
		GetMesh()->SetSkeletalMesh(tempMesh.Object);
		//1-3. Set Mesh location and rotation
		GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -90), FRotator(0, -90, 0));

		// EnemyFSM Component
		fsm = CreateDefaultSubobject<UEnemyFSM>(TEXT("FSM"));
	}
}

// Called when the game starts or when spawned
void AEnemyBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void AEnemyBase::Tick(float DeltaTime)
{
}

void AEnemyBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
}
