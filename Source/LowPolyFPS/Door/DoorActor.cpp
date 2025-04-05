// Fill out your copyright notice in the Description page of Project Settings.


#include "LowPolyFPS/Door/DoorActor.h"
#include "LowPolyFPS/Character/PlayerCharacter.h"
#include "GameFramework/Actor.h"
#include <Kismet/GameplayStatics.h>
#include "Components/BoxComponent.h"

// Sets default values
ADoorActor::ADoorActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Initialize components
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	DoorFrame = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorFrame"));
	DoorFrame->SetupAttachment(RootComponent);

	Door = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Door"));
	Door->SetupAttachment(DoorFrame);

	// Collision Box
	DoorCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("DoorCollision"));
	DoorCollision->SetupAttachment(DoorFrame);

	// Set Collision Response
	DoorCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly); // Only detect overlaps, no physics
	DoorCollision->SetCollisionResponseToAllChannels(ECR_Overlap); // Default to overlap for all
	DoorCollision->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block); // Block visibility channel

	DoorCollision->SetRelativeLocation(FVector(-115.0f, 10.0f, 0.0f));

	bIsDoorClosed = true;

}

// Called when the game starts or when spawned
void ADoorActor::BeginPlay()
{
	Super::BeginPlay();
	
	// Get player reference
	PlayerCharacter = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	if (CurveFloat)
	{
		FOnTimelineFloat TimelineProgress;
		TimelineProgress.BindDynamic(this, &ADoorActor::OpenDoor);
		Timeline.AddInterpFloat(CurveFloat, TimelineProgress);
	}

}

// Called every frame
void ADoorActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	Timeline.TickTimeline(DeltaTime);

}

void ADoorActor::OnInteract()
{
	if (Timeline.IsPlaying()) return; // Prevent multiple triggers while animation is running

	if (bIsDoorClosed)
	{
		SetDoorOnSameSide();
		Timeline.Play();
	}
	else {
		Timeline.Reverse();
	}

	bIsDoorClosed = !bIsDoorClosed; // Flip flop

}

void ADoorActor::OpenDoor(float Value)
{
	//float Angle = bDoorOnSameSide ? DoorRotateAngle : -DoorRotateAngle;
	float Angle = bDoorOnSameSide ? DoorRotateAngle : DoorRotateAngle;
	FRotator Rot = FRotator(0.0f, Angle * Value, 0.0f);
	Door->SetRelativeRotation(Rot);

}

void ADoorActor::SetDoorOnSameSide()
{
	if (!PlayerCharacter) return;

	FVector CharacterFV = PlayerCharacter->GetActorForwardVector();
	FVector DoorFV = GetActorForwardVector();
	bDoorOnSameSide = (FVector::DotProduct(CharacterFV, DoorFV) <= 0);

}

