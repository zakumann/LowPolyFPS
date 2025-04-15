// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Ladder.generated.h"

UCLASS()
class LOWPOLYFPS_API ALadder : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALadder();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ladder")
	class UBoxComponent* LadderTrigger;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ladder")
	class UStaticMeshComponent* LadderMesh;

	UFUNCTION(BlueprintCallable, Category = "Ladder")
	float GetHeight() const;
};
