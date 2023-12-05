// Fill out your copyright notice in the Description page of Project Settings.


#include "FPS/FPSGameState.h"
#include "Net/UnrealNetwork.h"
#include "FPS/FPSGlobalWidget.h"
#include "FPS/FPSPlayerController.h"
#include "FPS/FPSPlayerState.h"
#include "FPS/FPSCharacter.h"
#include "FPS/FPSGameMode.h"
#include "Multiplay/SessionSubsystem.h"

AFPSGameState::AFPSGameState()
{
	static ConstructorHelpers::FClassFinder<UFPSGlobalWidget> FPSGlobalWidgetAsset(TEXT("/Game/UI/FPS/BP_GlobalWidget.BP_GlobalWidget_C"));

	if (FPSGlobalWidgetAsset.Succeeded())
	{
		GlobalWidgetClass = FPSGlobalWidgetAsset.Class;
	}
}

void AFPSGameState::OnRep_KillCount()
{
	if (GlobalWidget)
	{
		GlobalWidget->UpdateGlobalScore(RedKill, BlueKill);
	}
}

void AFPSGameState::AddGlobalWidget()
{
	if (GlobalWidgetClass)
	{
		GlobalWidget = CreateWidget<UFPSGlobalWidget>(GetWorld(), GlobalWidgetClass);

		if (GlobalWidget)
		{
			GlobalWidget->AddToViewport();
		}
	}
}

void AFPSGameState::HandleMatchHasStarted()
{
	Super::HandleMatchHasStarted();

	AddGlobalWidget();
}

void AFPSGameState::HandleLeavingMap()
{
	GetGameInstance()->ReturnToMainMenu();
}

void AFPSGameState::OnPlayerDied(APlayerController* DeadPlayer, APlayerController* KillPlayer)
{
	auto KillPlayerController = Cast<AFPSPlayerController>(KillPlayer);

	FString DeadName = DeadPlayer->GetPlayerState<AFPSPlayerState>()->PlayerName;
	FString KillName = KillPlayer->GetPlayerState<AFPSPlayerState>()->PlayerName;
	UObject* WeaponIcon = KillPlayer->GetPawn<AFPSCharacter>()->GetWeaponIcon();
	
	if (KillPlayerController->GetTeam() == ETeam::RED)
	{
		RedKill++;
		NotifyKillLog(DeadName, KillName, WeaponIcon, true);
	}
	else
	{
		BlueKill++;
		NotifyKillLog(DeadName, KillName, WeaponIcon, false);
	}

	if (GlobalWidget)
	{
		GlobalWidget->UpdateGlobalScore(RedKill, BlueKill);
	}
}

void AFPSGameState::NotifyKillLog_Implementation(const FString& DeadName, const FString& KillName, UObject* WeaponIcon, bool bKillIsRed)
{
	if (GlobalWidget)
	{
		GlobalWidget->ShowKillLog(DeadName, KillName, WeaponIcon, bKillIsRed);
	}
}

void AFPSGameState::NotifyEndToPlayers()
{
	for (auto PlayerState : PlayerArray)
	{
		auto PlayerController = Cast<AFPSPlayerController>(PlayerState->GetPlayerController());
		
		if (PlayerController)
		{
			PlayerController->SetGameEndVisibility(RedKill > BlueKill);
		}
	}
}

void AFPSGameState::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	RedKill = 0;
	BlueKill = 0;
}

void AFPSGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AFPSGameState, RedKill);
	DOREPLIFETIME(AFPSGameState, BlueKill);
	DOREPLIFETIME(AFPSGameState, SessionName);
}
