// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "RoomController.generated.h"

/**
 * 
 */
UCLASS()
class RTSPROJECT_API ARoomController : public APlayerController
{
	GENERATED_BODY()

	TSubclassOf<class URoomWidget> RoomWidgetClass;
	class URoomWidget* RoomWidget;

public:
	ARoomController();

	UPROPERTY()
	bool bIsRedTeam;

protected:
	void BeginPlay() override;

public:
	void UpdateWidget();
	UFUNCTION(Server, Reliable)
	void Ready();
	void StartGame();
};
