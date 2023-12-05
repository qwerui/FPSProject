// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LobbyWidget.generated.h"

/**
 * 
 */
UCLASS()
class RTSPROJECT_API ULobbyWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta=(BindWidget))
	class UWidgetSwitcher* Switcher;
	UPROPERTY(meta = (BindWidget))
	class UWidgetSwitcher* JoinSwitcher;

	UPROPERTY(meta = (BindWidget))
	class UButton* HostButton;
	UPROPERTY(meta = (BindWidget))
	class UButton* JoinButton;
	UPROPERTY(meta = (BindWidget))
	class UButton* NameButton;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* PlayerName;
	
	TSubclassOf<class UJoinWidget> JoinWidgetClass;
	UPROPERTY(meta = (BindWidget))
	class UButton* BackButton;
	UPROPERTY(meta = (BindWidget))
	class UVerticalBox* RoomList;

	UPROPERTY(meta = (BindWidget))
	class UButton* CreateButton;
	UPROPERTY(meta = (BindWidget))
	class UButton* CancelButton;
	UPROPERTY(meta = (BindWidget))
	class UEditableTextBox* RoomName;

	UPROPERTY(meta = (BindWidget))
	class UButton* NameConfirmButton;
	UPROPERTY(meta = (BindWidget))
	class UButton* NameCancelButton;
	UPROPERTY(meta = (BindWidget))
	class UEditableTextBox* NewNameBox;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* CurrentName;

	ULobbyWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeOnInitialized() override;

private:
	UFUNCTION()
	void OnClick_HostButton();
	UFUNCTION()
	void OnClick_JoinButton();
	UFUNCTION() //CancelButton에도 적용 -> 초기 화면으로 이동
	void OnClick_BackButton();
	UFUNCTION()
	void OnClick_CreateButton();
	UFUNCTION()
	void OnClick_NameButton();
	UFUNCTION()
	void OnClick_NameConfirmButton();
	UFUNCTION()
	void FindSessionCompleted(bool bWasSuccessful);
	UFUNCTION()
	void NowJoining();
};
