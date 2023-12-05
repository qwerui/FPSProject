// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Engine.h"
#include "Net/UnrealNetwork.h"
#include "Online.h"
#include "SessionSubsystem.generated.h"

/**
 * 
 */
namespace SettingName
{
	const FName SETTING_ROOMNAME = FName(TEXT("RoomName"));
};

USTRUCT()
struct FSessionCreateParameter
{
	GENERATED_BODY()
public:
	FString RoomName;
	int MaxPlayer;
};

DECLARE_DELEGATE_OneParam(FSessionCompleteDelegate, bool);

UCLASS()
class RTSPROJECT_API USessionSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

	TSharedPtr<class FOnlineSessionSettings> SessionSettings;
	FDelegateHandle OnCreateSessionCompleteDelegateHandle;
	FDelegateHandle OnStartSessionCompleteDelegateHandle;
	FOnCreateSessionCompleteDelegate OnCreateSessionCompleteDelegate;
	FOnStartSessionCompleteDelegate OnStartSessionCompleteDelegate;

	TSharedPtr<class FOnlineSessionSearch> SessionSearch;
	FDelegateHandle OnFindSessionsCompleteDelegateHandle;
	FOnFindSessionsCompleteDelegate OnFindSessionsCompleteDelegate;

	FDelegateHandle OnJoinSessionCompleteDelegateHandle;
	FOnJoinSessionCompleteDelegate OnJoinSessionCompleteDelegate;

	FDelegateHandle OnDestroySessionCompleteDelegateHandle;
	FOnDestroySessionCompleteDelegate OnDestroySessionCompleteDelegate;
	
public:
	USessionSubsystem();

	FSessionCompleteDelegate OnStartSessionCallback;
	FSessionCompleteDelegate OnFindSessionCallback;
	FSessionCompleteDelegate OnJoinSessionCallback;
	FSessionCompleteDelegate OnDestroySessionCallback;
	
	bool CreateSession(TSharedPtr<const FUniqueNetId> UserId, FName SessionName, bool bIsLan, bool bIsPresence, FSessionCreateParameter Parameter);
	virtual void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);
	void OnStartOnlineGameComplete(FName SessionName, bool bWasSuccessful);

	bool FindSessions(TSharedPtr<const FUniqueNetId> UserId, bool bIsLan, bool bIsPresence);
	void OnFindSessionsComplete(bool bWasSuccessful);

	bool JoinSession(TSharedPtr<const FUniqueNetId> UserId, FName SessionName, const FOnlineSessionSearchResult& SearchResult);
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

	void DestroySessionAndLeaveGame(FName SessionName);
	virtual void OnDestroySessionComplete(FName SessionName, bool bWasSuccessful);

	const TArray<FOnlineSessionSearchResult> GetFoundSessions() const;

	virtual void Deinitialize() override;
};
