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
    PrimaryActorTick.bCanEverTick = false;

    LadderMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LadderMesh"));
    RootComponent = LadderMesh;

    LadderTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("LadderTrigger"));
    LadderTrigger->SetupAttachment(RootComponent);
    LadderTrigger->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    LadderTrigger->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
    LadderTrigger->SetCollisionResponseToChannel(ECC_Pawn, ECollisionResponse::ECR_Overlap);
}

float ALadder::GetHeight() const
{
    if (LadderMesh)
    {
        return LadderMesh->GetStaticMesh()->GetBounds().BoxExtent.Z * 2.0f * LadderMesh->GetComponentScale().Z;
    }

    return 0.f;
}