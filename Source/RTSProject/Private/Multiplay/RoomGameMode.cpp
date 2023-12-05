// Fill out your copyright notice in the Description page of Project Settings.


#include "Multiplay/RoomGameMode.h"
#include "Multiplay/RoomController.h"
#include "Multiplay/RoomGameState.h"
#include "Multiplay/RoomSession.h"

ARoomGameMode::ARoomGameMode()
{
    PlayerControllerClass = ARoomController::StaticClass();
    GameStateClass = ARoomGameState::StaticClass();
    PlayerStateClass = ARoomPlayerState::StaticClass();
    bUseSeamlessTravel = true;
}