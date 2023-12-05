// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "LocalDataSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class RTSPROJECT_API ULocalDataSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

	FText RoomName;
	FString PlayerName;

public:
	void SetRoomName(FText NewRoomName) {RoomName = NewRoomName;}
	FText GetRoomName() {return RoomName;}
	void SetPlayerName(FString NewPlayerName) { PlayerName = NewPlayerName; }
	FString GetPlayerName() { return PlayerName.Len() == 0 ? TEXT("Default Name") : PlayerName; }
};
