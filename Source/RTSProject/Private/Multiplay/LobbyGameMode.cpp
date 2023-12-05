// Fill out your copyright notice in the Description page of Project Settings.


#include "Multiplay/LobbyGameMode.h"
#include "Multiplay/LobbyController.h"
#include "Multiplay/LobbyWidget.h"

ALobbyGameMode::ALobbyGameMode()
{
    PlayerControllerClass = ALobbyController::StaticClass();

    static ConstructorHelpers::FClassFinder<ULobbyWidget> LobbyWidgetAsset(TEXT("/Game/UI/Lobby/Lobby.Lobby_C"));

    if(LobbyWidgetAsset.Succeeded())
    {
        LobbyWidgetClass = LobbyWidgetAsset.Class;
    }
}

void ALobbyGameMode::StartPlay()
{
    Super::StartPlay();
    
    if(LobbyWidgetClass)
    {
        LobbyWidget = CreateWidget<ULobbyWidget>(GetWorld(), LobbyWidgetClass);

        if(LobbyWidget)
        {
            LobbyWidget->AddToViewport();
        }
    }
}
