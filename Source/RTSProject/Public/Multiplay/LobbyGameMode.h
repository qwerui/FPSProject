// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "LobbyGameMode.generated.h"

/**
 * 
 */
UCLASS()
class RTSPROJECT_API ALobbyGameMode : public AGameModeBase
{
	GENERATED_BODY()

	TSubclassOf<class UUserWidget> LobbyWidgetClass;
	class ULobbyWidget* LobbyWidget;

public:
	ALobbyGameMode();
	virtual void StartPlay() override;
};
