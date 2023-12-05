// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "DeadPawn.generated.h"

UCLASS()
class RTSPROJECT_API ADeadPawn : public APawn
{
	GENERATED_BODY()

	UPROPERTY(Replicated)
	FVector TargetPosition;
	UPROPERTY(Replicated)
	FVector OriginPosition;
	AActor* BeforePawn;
	float Lerp;
	const float MoveTime = 3.0f;

public:
	// Sets default values for this pawn's properties
	ADeadPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void Destroyed() override;

	void SetBeforePawn(AActor* NewBeforePawn);

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
