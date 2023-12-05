// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "FPSPlayerState.generated.h"

/**
 * 
 */

UCLASS()
class RTSPROJECT_API AFPSPlayerState : public APlayerState
{
	GENERATED_BODY()

	UPROPERTY(Replicated, Transient, meta = (ClampMin = 0))
	float RemainRespawnTime;
	UPROPERTY(Replicated)
	int8 Kill;
	UPROPERTY(Replicated)
	int8 Death;

public:
	AFPSPlayerState();

	UPROPERTY(Replicated)
	FString PlayerName;

	bool CanRespawn() { return RemainRespawnTime <= 0; }
	void SetRespawnTime(float NewRespawnTime) { RemainRespawnTime = NewRespawnTime; }
	float GetRespawnTime() const { return RemainRespawnTime; }

	virtual void Tick(float DeltaSeconds) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
