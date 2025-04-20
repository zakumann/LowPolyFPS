// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EnemyFSM.generated.h"

UENUM(BlueprintType)
enum class EEnemyState : uint8
{
	Idle,
	Move,
	Attack,
	Damage,
	Die,
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LOWPOLYFPS_API UEnemyFSM : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UEnemyFSM();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	// Status Variable
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=FSM)
	EEnemyState mState = EEnemyState::Idle;

	// Idle State
	void IdleState();

	// Move State
	void MoveState();

	// Attack State
	void AttackState();

	// Damage State
	void DamageState();

	// Die State
	void DieState();

	// Delay
	UPROPERTY(EditDefaultsOnly, Category=FSM)
	float idleDelayTime = 2;
	// Loading
	float currentTime = 0;

	// Target
	UPROPERTY(VisibleAnywhere, Category = FSM)
	class APlayerCharacter* target;

	// Actor property
	UPROPERTY()
	class AEnemyBase* me;

	// Attack Range
	UPROPERTY(EditAnywhere, Category = FSM)
	float attackRange = 150.0f;

	// Attack delay
	UPROPERTY(EditAnywhere, Category = FSM)
	float attackDelayTime = 2.0f;

	// Damage message
	void OnDamageProcess();

	//Heatlh
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=FSM)
	int32 hp = 3;

	//Damage delay
	UPROPERTY(EditAnywhere, Category=FSM)
	float damageDelayTime = 2.0f;

	// disappear time
	UPROPERTY(EditAnywhere, Category=FSM)
	float dieSpeed = 50.0f;
};
