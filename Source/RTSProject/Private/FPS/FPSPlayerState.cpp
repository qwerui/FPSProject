// Fill out your copyright notice in the Description page of Project Settings.


#include "FPS/FPSPlayerState.h"
#include "Net/UnrealNetwork.h"

AFPSPlayerState::AFPSPlayerState()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AFPSPlayerState::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (GetLocalRole() == ENetRole::ROLE_Authority)
	{
		RemainRespawnTime -= DeltaSeconds;
	}
}

void AFPSPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AFPSPlayerState, RemainRespawnTime);
	DOREPLIFETIME(AFPSPlayerState, Kill);
	DOREPLIFETIME(AFPSPlayerState, Death);
	DOREPLIFETIME(AFPSPlayerState, PlayerName);
}
