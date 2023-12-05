// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameSession.h"
#include "RoomSession.generated.h"

/**
 * 
 */
UCLASS()
class RTSPROJECT_API ARoomSession : public AGameSession
{
	GENERATED_BODY()

public:
	virtual void UnregisterPlayer(const APlayerController* ExitingPlayer) override;
};
