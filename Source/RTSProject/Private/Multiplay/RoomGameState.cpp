// Fill out your copyright notice in the Description page of Project Settings.


#include "Multiplay/RoomGameState.h"
#include "System/LocalDataSubsystem.h"
#include "Multiplay/SessionSubsystem.h"
#include "Multiplay/RoomController.h"
#include "Multiplay/RoomPlayerState.h"
#include "Net/UnrealNetwork.h"


ARoomGameState::ARoomGameState()
{
    
}

void ARoomGameState::HandleBeginPlay()
{
    auto GameInstance = GetWorld()->GetGameInstance();
    auto Session = GameInstance->GetSubsystem<USessionSubsystem>();
    auto LocalData = GameInstance->GetSubsystem<ULocalDataSubsystem>();
    ULocalPlayer* const Player = GameInstance->GetFirstGamePlayer();

    RoomName = LocalData->GetRoomName();
    MaxPlayer = 4;

    Super::HandleBeginPlay();

    FSessionCreateParameter Parameter;
    Parameter.RoomName = LocalData -> GetRoomName().ToString();
    Parameter.MaxPlayer = 4;

    Session->CreateSession(Player->GetPreferredUniqueNetId().GetUniqueNetId(),NAME_GameSession,false,false,Parameter);
}

void ARoomGameState::RemovePlayerState(APlayerState* PlayerState)
{
    Super::RemovePlayerState(PlayerState);

    OnPlayerLeave();
}

void ARoomGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(ARoomGameState, RoomName);
}

void ARoomGameState::RequestPlayerLeave_Implementation()
{
    OnPlayerLeave();
}

void ARoomGameState::OnPlayerLeave_Implementation()
{
    GetWorld()->GetFirstPlayerController<ARoomController>()->UpdateWidget();
}


void ARoomGameState::DivideTeam()
{
    int Red = 0;
    int Blue = 0;

    for (APlayerState* It : PlayerArray)
    {
        auto RoomPlayer = Cast<ARoomController>(It->GetPlayerController());

        if (RoomPlayer)
        {
            if (Red >= PlayerArray.Num()/2)
            {
                RoomPlayer->bIsRedTeam = false;
            }
            else if(Blue >= PlayerArray.Num()/2)
            {
                RoomPlayer->bIsRedTeam = true;
            }
            else
            {
                bool bIsRed = FMath::RandBool();

                if (bIsRed)
                {
                    RoomPlayer->bIsRedTeam = true;
                    Red++;
                }
                else
                {
                    RoomPlayer->bIsRedTeam = false;
                    Blue++;
                }
            }
        }
    }
}

void ARoomGameState::OnRep_RoomName()
{
    GetWorld()->GetFirstPlayerController<ARoomController>()->UpdateWidget();
}

void ARoomGameState::OnRep_MaxPlayer()
{
    GetWorld()->GetFirstPlayerController<ARoomController>()->UpdateWidget();
}
