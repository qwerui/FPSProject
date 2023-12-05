// Fill out your copyright notice in the Description page of Project Settings.


#include "Multiplay/RoomPlayerState.h"
#include "System/LocalDataSubsystem.h"
#include "Multiplay/RoomController.h"
#include "GameframeWork/GameMode.h"
#include "FPS/FPSPlayerState.h"
#include "Net/UnrealNetwork.h"

void ARoomPlayerState::ClientInitialize(AController* C)
{
	Super::ClientInitialize(C);

	auto LocalData = GetWorld()->GetGameInstance()->GetSubsystem<ULocalDataSubsystem>();
	SendPlayerNameFromClient(LocalData->GetPlayerName());
}

void ARoomPlayerState::SeamlessTravelTo(APlayerState* NewPlayerState)
{
	auto FPSState = Cast<AFPSPlayerState>(NewPlayerState);
	if (FPSState)
	{
		FPSState->PlayerName = PlayerName;
		Super::SeamlessTravelTo(FPSState);
	}
	else
	{
		Super::SeamlessTravelTo(NewPlayerState);
	}
	
}

void ARoomPlayerState::OnRep_PlayerNickName()
{
	GetWorld()->GetFirstPlayerController<ARoomController>()->UpdateWidget();
}

void ARoomPlayerState::OnRep_bIsReady()
{
	GetWorld()->GetFirstPlayerController<ARoomController>()->UpdateWidget();
}

void ARoomPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ARoomPlayerState, PlayerName);
	DOREPLIFETIME(ARoomPlayerState, bIsReady);
}

void ARoomPlayerState::SendPlayerNameFromClient_Implementation(const FString& NewPlayerName)
{
	PlayerName = NewPlayerName;
	GetWorld()->GetFirstPlayerController<ARoomController>()->UpdateWidget();
}
