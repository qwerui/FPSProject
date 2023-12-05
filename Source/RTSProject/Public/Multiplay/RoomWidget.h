// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "RoomWidget.generated.h"

/**
 * 
 */
UCLASS()
class RTSPROJECT_API URoomWidget : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(meta = (BindWidget))
	UButton* BackButton;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* RoomName;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> ConfirmButton;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ConfirmButtonLabel;
	
	UPROPERTY(meta = (BindWidget))
	class UVerticalBox* PlayerInfoList;
	TSubclassOf<class UPlayerInfoWidget> PlayerInfoClass;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> MaxPlayer;

	int ConnectedPlayer;

public:
	URoomWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeOnInitialized() override;
	void CreatePlayerInfo(int index, FString PlayerName, bool bIsLeader);
	void RemovePlayerInfo(int PlayerCount);
	void SetReady(int PlayerIndex, bool bIsReady);

	void SetRoomName(FText NewRoomName) {RoomName->SetText(NewRoomName);}
	void SetMaxPlayer(FText NewMaxPlayer) { MaxPlayer->SetText(NewMaxPlayer); }

private:
	UFUNCTION()
	void OnClick_BackButton();
	UFUNCTION()
	void OnClick_ConfirmButton();
};
