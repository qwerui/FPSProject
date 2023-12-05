// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerInfoWidget.generated.h"

/**
 * 
 */
UCLASS()
class RTSPROJECT_API UPlayerInfoWidget : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY()
	class USlateBrushAsset* LeaderSprite;
	UPROPERTY()
	class USlateBrushAsset* ReadySprite;

	UPROPERTY()
	class UImage* ReadyCheckImage;
	UPROPERTY()
	class UTextBlock* PlayerNameText;

public:
	UPlayerInfoWidget(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativeOnInitialized() override;

public:
	void Init(FString PlayerName, bool bIsLeader);
	void CheckReady(bool bIsReady);
};
