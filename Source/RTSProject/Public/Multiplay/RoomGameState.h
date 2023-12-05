// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "RoomGameState.generated.h"

/**
 * 
 */
UCLASS()
class RTSPROJECT_API ARoomGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	UPROPERTY(ReplicatedUsing = OnRep_RoomName)
	FText RoomName;
	UPROPERTY(ReplicatedUsing = OnRep_MaxPlayer)
	int MaxPlayer;

	ARoomGameState();
	virtual void HandleBeginPlay() override; //Server Only
	virtual void RemovePlayerState(APlayerState* PlayerState) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	UFUNCTION(NetMulticast, Reliable)
	void OnPlayerLeave();
	UFUNCTION(Server, Reliable)
	void RequestPlayerLeave();
	void DivideTeam();
private:
	UFUNCTION()
	void OnRep_RoomName();
	UFUNCTION()
	void OnRep_MaxPlayer();
};
