// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "FPSGameState.generated.h"

/**
 * 
 */
UCLASS()
class RTSPROJECT_API AFPSGameState : public AGameState
{
	GENERATED_BODY()

	TSubclassOf<class UFPSGlobalWidget> GlobalWidgetClass;
	UPROPERTY();
	TObjectPtr<class UFPSGlobalWidget> GlobalWidget;

	UPROPERTY(ReplicatedUsing = OnRep_KillCount, Transient)
	int8 RedKill;
	UPROPERTY(ReplicatedUsing = OnRep_KillCount, Transient)
	int8 BlueKill;

	UPROPERTY(Replicated, Transient)
	FName SessionName;

public:
	AFPSGameState();

private:
	UFUNCTION()
	void OnRep_KillCount();

	void AddGlobalWidget();

protected:
	virtual void HandleMatchHasStarted() override;
	virtual void HandleLeavingMap() override;

public:
	void OnPlayerDied(APlayerController* DeadPlayer, APlayerController* KillPlayer);
	UFUNCTION(NetMulticast, Reliable)
	void NotifyKillLog(const FString& DeadName, const FString& KillName, UObject* WeaponIcon, bool bKillIsRed);
	bool CheckFinish() const { return RedKill >= 5 || BlueKill >= 5; }
	void NotifyEndToPlayers();

	void SetSessionName(FName NewSessionName) { SessionName = NewSessionName; }

	virtual void PostInitializeComponents() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
