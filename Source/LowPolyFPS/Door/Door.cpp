// Fill out your copyright notice in the Description page of Project Settings.


#include "LowPolyFPS/Door/Door.h"
#include "LowPolyFPS/Character/PlayerCharacter.h"
#include <Kismet/GameplayStatics.h>

// Sets default values
ADoor::ADoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Initialize components
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	DoorFrame = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorFrame"));
	DoorFrame->SetupAttachment(RootComponent);

	Door = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Door"));
	Door->SetupAttachment(DoorFrame);

	bIsDoorClosed = true;
}

// Called when the game starts or when spawned
void ADoor::BeginPlay()
{
	Super::BeginPlay();

	// Get player reference
	PlayerCharacter = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	
	if (CurveFloat)
	{
		FOnTimelineFloat TimelineProgress;
		TimelineProgress.BindDynamic(this, &ADoor::OpenDoor);
		Timeline.AddInterpFloat(CurveFloat, TimelineProgress);
	}
}

// Called every frame
void ADoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Timeline.TickTimeline(DeltaTime);
}

void ADoor::OnInteract()
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

void ADoor::OpenDoor(float Value)
{
	//float Angle = bDoorOnSameSide ? DoorRotateAngle : -DoorRotateAngle;
	float Angle = bDoorOnSameSide ? DoorRotateAngle : DoorRotateAngle;

	FRotator Rot = FRotator(0.0f, Angle * Value, 0.0f);

	Door->SetRelativeRotation(Rot);
}

void ADoor::SetDoorOnSameSide()
{
	if (!PlayerCharacter) return;

	FVector CharacterFV = PlayerCharacter->GetActorForwardVector();
	FVector DoorFV = GetActorForwardVector();
	bDoorOnSameSide = (FVector::DotProduct(CharacterFV, DoorFV) <= 0);
}

