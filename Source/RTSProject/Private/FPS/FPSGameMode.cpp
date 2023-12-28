// Fill out your copyright notice in the Description page of Project Settings.


#include "FPS/FPSGameMode.h"
#include "FPS/FPSCharacter.h"
#include "FPS/FPSGameState.h"
#include "FPS/FPSPlayerState.h"
#include "FPS/FPSPlayerController.h"
#include "GameFramework/GameSession.h"
#include "Multiplay/SessionSubsystem.h"

AFPSGameMode::AFPSGameMode()
{
	DefaultPawnClass = AFPSCharacter::StaticClass();
	GameStateClass = AFPSGameState::StaticClass();
	PlayerStateClass = AFPSPlayerState::StaticClass();
	PlayerControllerClass = AFPSPlayerController::StaticClass();

	MinRespawnDelay = 5.0f;
}

void AFPSGameMode::HandleMatchHasStarted()
{
	Super::HandleMatchHasStarted();

	SessionName = GameSession->SessionName;
	GetGameState<AFPSGameState>()->SetSessionName(SessionName);
}

void AFPSGameMode::HandleMatchHasEnded()
{
	GetGameState<AFPSGameState>()->NotifyEndToPlayers();

	Super::HandleMatchHasEnded();

	FTimerManager& WorldTimer = GetWorld()->GetTimerManager();
	WorldTimer.SetTimer(LeavingHandle, [&]()->void {
		StartToLeaveMap();
		GetWorld()->GetTimerManager().ClearTimer(LeavingHandle);
	}, 3.0f, false);
}

void AFPSGameMode::Respawn(APlayerController* RespawnTarget)
{
	auto DeadPawn = RespawnTarget->GetPawn();
	RespawnTarget->UnPossess();
	DeadPawn->Destroy();
	RestartPlayer(RespawnTarget);
	Cast<AFPSPlayerController>(RespawnTarget)->SetDeadWidgetVisibility(false);
}

void AFPSGameMode::PlayerDied(APlayerController* DeadPlayer, APlayerController* KillPlayer)
{
	check(DeadPlayer != nullptr);
	check(KillPlayer != nullptr);

	auto FPSGameState = GetGameState<AFPSGameState>();
	FPSGameState->OnPlayerDied(DeadPlayer, KillPlayer);

	if (FPSGameState->CheckFinish())
	{
		EndMatch();
	}
	else
	{
		DeadPlayer->GetPlayerState<AFPSPlayerState>()->SetRespawnTime(MinRespawnDelay);
		RespawnQueue.Enqueue(DeadPlayer);
		Cast<AFPSPlayerController>(DeadPlayer)->SetDeadWidgetVisibility(true);
	}
}

void AFPSGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	APlayerController* RespawnTarget = nullptr;

	if (RespawnQueue.Peek(RespawnTarget))
	{
		if (IsValid(RespawnTarget))
		{
			auto RespawnTargetState = RespawnTarget->GetPlayerState<AFPSPlayerState>();

			if (RespawnTargetState->CanRespawn())
			{
				Respawn(RespawnTarget);
				RespawnQueue.Pop();
			}
		}
	}
}

void AFPSGameMode::RestartPlayer(AController* NewPlayer)
{
	if (NewPlayer == nullptr || NewPlayer->IsPendingKillPending())
	{
		return;
	}

	auto FPSController = Cast<AFPSPlayerController>(NewPlayer);

	check(FPSController != nullptr);

	AActor* StartSpot = nullptr;

	if (FPSController->GetTeam() == ETeam::RED)
	{
		StartSpot = FindPlayerStart(NewPlayer, TEXT("Red"));
	}
	else
	{
		StartSpot = FindPlayerStart(NewPlayer, TEXT("Blue"));
	}
	
	check(StartSpot != nullptr);
	
	RestartPlayerAtPlayerStart(NewPlayer, StartSpot);
}
