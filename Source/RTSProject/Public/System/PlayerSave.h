// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "PlayerSave.generated.h"

/**
 * 
 */
UCLASS()
class RTSPROJECT_API UPlayerSave : public USaveGame
{
	GENERATED_BODY()
	
public:
	UPlayerSave();

	FString SaveSlotName;
	uint32 UserIndex;

	UPROPERTY()
	FString PlayerName;
};
