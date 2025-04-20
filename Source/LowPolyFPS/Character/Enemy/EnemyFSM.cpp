// Fill out your copyright notice in the Description page of Project Settings.


#include "LowPolyFPS/Character/Enemy/EnemyFSM.h"
#include "LowPolyFPS/Character/PlayerCharacter.h"
#include "LowPolyFPS/Character/Enemy/EnemyBase.h"
#include "Kismet/GameplayStatics.h"
#include "LowPolyFPS/LowPolyFPS.h"
#include "Components/CapsuleComponent.h"

// Sets default values for this component's properties
UEnemyFSM::UEnemyFSM()
{
	PrimaryComponentTick.bCanEverTick = true;

}


// Called when the game starts
void UEnemyFSM::BeginPlay()
{
	Super::BeginPlay();
	
	auto actor = UGameplayStatics::GetActorOfClass(GetWorld(), APlayerCharacter::StaticClass());
	// Cast APlayerCharacter type
	target = Cast<APlayerCharacter>(actor);
	//Bring proprietary Object
	me = Cast<AEnemyBase>(GetOwner());
}


// Called every frame
void UEnemyFSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FString logMsg = UEnum::GetValueAsString(mState);
	GEngine->AddOnScreenDebugMessage(0, 1, FColor::Cyan, logMsg);

	switch (mState)
	{
	case EEnemyState::Idle:
		IdleState();
		break;
	case EEnemyState::Move:
		MoveState();
		break;
	case EEnemyState::Attack:
		AttackState();
		break;
	case EEnemyState::Damage:
		DamageState();
		break;
	case EEnemyState::Die:
		DieState();
		break;
	}
}

// Idle State
void UEnemyFSM::IdleState()
{
	//1. pass the Time
	currentTime += GetWorld()->DeltaTimeSeconds;
	//2. if Time has done...
	if (currentTime > idleDelayTime)
	{
		// 3. Turn to the Move State
		mState = EEnemyState::Move;
		//pass the current time
		currentTime = 0;
	}
}
// Move State
void UEnemyFSM::MoveState()
{
	// 1. need destination to the Target
	FVector destination = target->GetActorLocation();
	// 2. need direction
	FVector dir = destination - me->GetActorLocation();
	// 3. Move to the direction
	me->AddMovementInput(dir.GetSafeNormal());

	// If close with target turn on attack
	//1. if inside in the attack range...
	if (dir.Size() < attackRange)
	{
		//2. Turn to the attack
		mState = EEnemyState::Attack;
	}
}
// Attack State
void UEnemyFSM::AttackState()
{
	// Object: Attack with delay time
	//1. DeltaTime
	currentTime += GetWorld()->DeltaTimeSeconds;
	// 2. if Attack Delay is done...
	if (currentTime > attackDelayTime)
	{
		//3. I wish Attack
		UE_LOG(LogTemp, Warning, TEXT("Attack"));
		// back to 0
		currentTime = 0;
	}

	// Object: if Target escape from attack range, turn on move state...
	// 1. need distance with target
	float distance = FVector::Distance(target->GetActorLocation(), me->GetActorLocation());

	//2. if target out of attack ragne
	if (distance > attackRange)
	{
		// 3, turn move state
		mState = EEnemyState::Move;
	}
}
// Damage State
void UEnemyFSM::DamageState() 
{
	//1. Time pass
	currentTime += GetWorld()->DeltaTimeSeconds;
	//2. if currentTime over delayTime
	if (currentTime > damageDelayTime)
	{
		//3. turn Idle time
		mState = EEnemyState::Idle;
		// turn currentTime to 0
		currentTime = 0;
	}
}
// Die State
void UEnemyFSM::DieState()
{
	// disappeared toward under
	// P = P0 + vt
	FVector P0 = me->GetActorLocation();
	FVector vt = FVector::DownVector * dieSpeed * GetWorld()->DeltaTimeSeconds;
	FVector P = P0 + vt;
	me->SetActorLocation(P);

	//1. if object downs over two meters...
	if (P.Z < -200.0f)
	{
		//Destroy
		me->Destroy();
	}
}

void UEnemyFSM::OnDamageProcess()
{
	// Decrease Health
	hp--;

	// if Health still remains
	if (hp > 0)
	{
		mState = EEnemyState::Damage;
	}
	//otherwise
	else
	{
		//Died
		mState = EEnemyState::Die;
		// Deactivate Capsule Collision
		me->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}
