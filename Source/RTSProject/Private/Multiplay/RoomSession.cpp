// Fill out your copyright notice in the Description page of Project Settings.


#include "Multiplay/RoomSession.h"
#include "Multiplay/RoomGameState.h"

void ARoomSession::UnregisterPlayer(const APlayerController* ExitingPlayer)
{
	Super::UnregisterPlayer(ExitingPlayer);
}
