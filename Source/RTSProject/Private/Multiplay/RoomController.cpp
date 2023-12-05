// Fill out your copyright notice in the Description page of Project Settings.


#include "Multiplay/RoomController.h"
#include "System/LocalDataSubsystem.h"
#include "Multiplay/RoomGameState.h"
#include "Net/UnrealNetwork.h"
#include "Multiplay/RoomWidget.h"
#include "Multiplay/RoomPlayerState.h"
#include "Multiplay/PlayerInfoWidget.h"

ARoomController::ARoomController()
{
    bShowMouseCursor = true;

    static ConstructorHelpers::FClassFinder<URoomWidget> RoomWidgetAsset(TEXT("/Game/UI/Room/Room.Room_C"));

    if (RoomWidgetAsset.Succeeded())
    {
        RoomWidgetClass = RoomWidgetAsset.Class;
    }
}

void ARoomController::BeginPlay()
{
    Super::BeginPlay();

    if (HasAuthority()) //Server
    {
        if (IsLocalController())
        {
            auto LocalData = GetWorld()->GetGameInstance()->GetSubsystem<ULocalDataSubsystem>();
            auto RoomPlayerState = GetPlayerState<ARoomPlayerState>();
            RoomPlayerState->PlayerName = LocalData->GetPlayerName();
            RoomPlayerState->bIsReady = true;

            UpdateWidget();
        }
    }
    else //Client
    {
        UpdateWidget();
    }
}

void ARoomController::UpdateWidget()
{
    if (RoomWidget)
    {
        
        TObjectPtr<ARoomGameState> GameState = GetWorld()->GetGameState<ARoomGameState>();
        
        if(GameState)
        {
            RoomWidget->SetRoomName(GameState->RoomName);

            for (int i = 0; i < GameState->PlayerArray.Num(); i++)
            {
                TObjectPtr<ARoomPlayerState> RoomPlayerState = Cast<ARoomPlayerState>(GameState->PlayerArray[i]);
                RoomWidget->CreatePlayerInfo(i, RoomPlayerState->PlayerName, i == 0); //Listen Server에서 0번은 호스트 것 == 방장
                RoomWidget->SetReady(i, RoomPlayerState->bIsReady);
            }

            RoomWidget->RemovePlayerInfo(GameState->PlayerArray.Num());
        }
    }
    else
    {
        if (RoomWidgetClass)
        {
            RoomWidget = CreateWidget<URoomWidget>(GetWorld(), RoomWidgetClass);

            if (RoomWidget)
            {
                RoomWidget->AddToViewport();
                UpdateWidget();
            }
        }
    }   
}

void ARoomController::StartGame()
{
    bool bIsAllReady = true;

    for (TObjectPtr<APlayerState> PlayerStateFromArray : GetWorld()->GetGameState()->PlayerArray)
    {
        TObjectPtr<ARoomPlayerState> RoomPlayerState = Cast<ARoomPlayerState>(PlayerStateFromArray);
        bIsAllReady = bIsAllReady && RoomPlayerState->bIsReady;
    }

    if (bIsAllReady)
    {
        GetWorld()->ServerTravel(TEXT("FirstMap"));
    }
}

void ARoomController::Ready_Implementation()
{
    auto RoomPlayerState = GetPlayerState<ARoomPlayerState>();
    RoomPlayerState->bIsReady = !RoomPlayerState->bIsReady;
    UpdateWidget();
}



