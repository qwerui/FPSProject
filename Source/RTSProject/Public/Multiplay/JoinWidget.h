// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Multiplay/SessionSubsystem.h"
#include "JoinWidget.generated.h"

/**
 * 
 */
DECLARE_DELEGATE(FJoinDelegate);

UCLASS()
class RTSPROJECT_API UJoinWidget : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* JoinRoomName;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* JoinPlayerCount;
	UPROPERTY(meta = (BindWidget))
	class UButton* JoinPlayer;

	FOnlineSessionSearchResult SessionForJoin;

protected:
	virtual void NativeOnInitialized() override;
	UFUNCTION()
	void OnClick_JoinPlayer();
	UFUNCTION()
	void OnJoin(bool bWasSuccessful);

public:
	FJoinDelegate OnJoinDelegate;

	void SetSession(class FOnlineSessionSearchResult FoundSession);
};
