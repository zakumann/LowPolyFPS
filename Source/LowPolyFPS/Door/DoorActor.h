// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/TimelineComponent.h"
#include "DoorActor.generated.h"

UCLASS()
class LOWPOLYFPS_API ADoorActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADoorActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void OnInteract();

	class APlayerCharacter* PlayerCharacter;

private:
	UPROPERTY(VisibleAnywhere, Category = "Door Components")
	class UStaticMeshComponent* DoorFrame;

	UPROPERTY(VisibleAnywhere, Category = "Door Components")
	class UStaticMeshComponent* Door;

	UPROPERTY(VisibleAnywhere, Category = "Door Components")
	class UBoxComponent* DoorCollision;

protected:
	// Timeline for door movement
	UPROPERTY()
	FTimeline Timeline;

	UPROPERTY(EditAnywhere)
	class UCurveFloat* CurveFloat;

	bool bIsDoorClosed = true;

	UPROPERTY(EditAnywhere)
	float DoorRotateAngle = 90.f;

	UFUNCTION()
	void OpenDoor(float Value); // Bind function

	bool bDoorOnSameSide = false;
	void SetDoorOnSameSide();

};