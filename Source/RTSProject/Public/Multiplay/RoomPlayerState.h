// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "RoomPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class RTSPROJECT_API ARoomPlayerState : public APlayerState
{
	GENERATED_BODY()

	UPROPERTY()
	bool bIsRedTeam;
	
public:
	UPROPERTY(ReplicatedUsing = OnRep_PlayerNickName)
	FString PlayerName;
	UPROPERTY(ReplicatedUsing = OnRep_bIsReady)
	bool bIsReady;

	virtual void ClientInitialize(AController* C) override;
	virtual void SeamlessTravelTo(APlayerState* NewPlayerState) override;

	UFUNCTION(Server, Reliable)
	void SendPlayerNameFromClient(const FString& NewPlayerName);

private:
	UFUNCTION()
	void OnRep_PlayerNickName();
	UFUNCTION()
	void OnRep_bIsReady();

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
