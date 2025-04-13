// Fill out your copyright notice in the Description page of Project Settings.


#include "Ladder.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "LowPolyFPS/Character/PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
ALadder::ALadder()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create and set root component
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	// Create ladder mesh
	LadderMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LadderMesh"));
	LadderMesh->SetupAttachment(RootComponent);

	// Create trigger box
	LadderTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("LadderTrigger"));
	LadderTrigger->SetupAttachment(RootComponent);
	LadderTrigger->SetBoxExtent(FVector(50.f, 50.f, 200.f));
	LadderTrigger->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	LadderTrigger->SetCollisionResponseToAllChannels(ECR_Ignore);
	LadderTrigger->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}

// Called when the game starts or when spawned
void ALadder::BeginPlay()
{
	Super::BeginPlay();
	
	LadderTrigger->OnComponentBeginOverlap.AddDynamic(this, &ALadder::OnOverlapBegin);
	LadderTrigger->OnComponentEndOverlap.AddDynamic(this, &ALadder::OnOverlapEnd);
}

void ALadder::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// If player is climbing, check if they are still inside the ladder trigger
	if (IsValid(CurrentPlayer) && CurrentPlayer->bIsOnLadder)
	{
		if (!LadderTrigger->IsOverlappingActor(CurrentPlayer))
		{
			// Player moved outside the trigger while climbing - force them off the ladder
			CurrentPlayer->bIsOnLadder = false;
			CurrentPlayer->GetCharacterMovement()->SetMovementMode(MOVE_Walking);
			CurrentPlayer = nullptr;
		}
	}
}

void ALadder::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (APlayerCharacter* Player = Cast<APlayerCharacter>(OtherActor))
	{
		Player->CurrentLadder = this;
		Player->LadderEnterLocation = Player->GetActorLocation(); // <--- store local reference too
		CurrentPlayer = Player;
	}
}

void ALadder::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (APlayerCharacter* Player = Cast<APlayerCharacter>(OtherActor))
	{
		Player->CurrentLadder = nullptr; // Clear ladder reference if out of range

		// Optional: auto stop climbing if still active
		if (Player->bIsOnLadder)
		{
			Player->bIsOnLadder = false;
			Player->GetCharacterMovement()->SetMovementMode(MOVE_Walking);
		}

		if (Player == CurrentPlayer)
		{
			CurrentPlayer = nullptr;
		}
	}
}