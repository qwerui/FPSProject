// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "FPSGameMode.generated.h"

/**
 * 
 */
UCLASS()
class RTSPROJECT_API AFPSGameMode : public AGameMode
{
	GENERATED_BODY()

	TQueue<class APlayerController*> RespawnQueue;
	FTimerHandle LeavingHandle;
	FName SessionName;

public:
	AFPSGameMode();

protected:
	virtual void HandleMatchHasStarted() override;
	virtual void HandleMatchHasEnded() override;

private:
	void Respawn(class APlayerController* RespawnTarget);

public:
	void PlayerDied(class APlayerController* DeadPlayer, class APlayerController* KillPlayer);

	virtual void Tick(float DeltaSeconds) override;
	virtual void RestartPlayer(AController* NewPlayer) override;

	FName GetSessionName() const { return SessionName; }
};
