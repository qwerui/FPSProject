// Fill out your copyright notice in the Description page of Project Settings.


#include "Multiplay/SessionSubsystem.h"

USessionSubsystem::USessionSubsystem()
{
    OnFindSessionsCompleteDelegate = FOnFindSessionsCompleteDelegate::CreateUObject(this, &USessionSubsystem::OnFindSessionsComplete);
    OnCreateSessionCompleteDelegate = FOnCreateSessionCompleteDelegate::CreateUObject(this, &USessionSubsystem::OnCreateSessionComplete);
	OnStartSessionCompleteDelegate = FOnStartSessionCompleteDelegate::CreateUObject(this, &USessionSubsystem::OnStartOnlineGameComplete);
	OnJoinSessionCompleteDelegate = FOnJoinSessionCompleteDelegate::CreateUObject(this, &USessionSubsystem::OnJoinSessionComplete);
	OnDestroySessionCompleteDelegate = FOnDestroySessionCompleteDelegate::CreateUObject(this, &USessionSubsystem::OnDestroySessionComplete);
}

bool USessionSubsystem::CreateSession(TSharedPtr<const FUniqueNetId> UserId, FName SessionName, bool bIsLan, bool bIsPresence, FSessionCreateParameter Parameter)
{
    IOnlineSubsystem* const OnlineSub = IOnlineSubsystem::Get();

	if (OnlineSub)
	{
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();

		if (Sessions.IsValid() && UserId.IsValid())
		{
			SessionSettings = MakeShareable(new FOnlineSessionSettings());

			SessionSettings->bIsLANMatch = bIsLan;
			SessionSettings->bUsesPresence = bIsPresence;
			SessionSettings->NumPublicConnections = Parameter.MaxPlayer - 1;
			SessionSettings->NumPrivateConnections = 0;
			SessionSettings->bAllowInvites = true;
			SessionSettings->bAllowJoinInProgress = true;
			SessionSettings->bShouldAdvertise = true;
			SessionSettings->bAllowJoinViaPresence = true;
			SessionSettings->bAllowJoinViaPresenceFriendsOnly = false;

			SessionSettings->Set(SettingName::SETTING_ROOMNAME, Parameter.RoomName, EOnlineDataAdvertisementType::ViaOnlineService);

			OnCreateSessionCompleteDelegateHandle = Sessions->AddOnCreateSessionCompleteDelegate_Handle(OnCreateSessionCompleteDelegate);

			return Sessions->CreateSession(*UserId, SessionName, *SessionSettings);
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, TEXT("No OnlineSubsytem found!"));
	}

	return false;
}

void USessionSubsystem::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
    GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("OnCreateSessionComplete %s, %d"), *SessionName.ToString(), bWasSuccessful));

	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
	if (OnlineSub)
	{
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();

		if (Sessions.IsValid())
		{
			Sessions->ClearOnCreateSessionCompleteDelegate_Handle(OnCreateSessionCompleteDelegateHandle);
			if (bWasSuccessful)
			{
				OnStartSessionCompleteDelegateHandle = Sessions->AddOnStartSessionCompleteDelegate_Handle(OnStartSessionCompleteDelegate);
				Sessions->StartSession(SessionName);
			}
		}
		
	}
}
void USessionSubsystem::OnStartOnlineGameComplete(FName SessionName, bool bWasSuccessful)
{
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
	if (OnlineSub)
	{
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();
		if (Sessions.IsValid())
		{
			Sessions->ClearOnStartSessionCompleteDelegate_Handle(OnStartSessionCompleteDelegateHandle);
			OnStartSessionCallback.ExecuteIfBound(bWasSuccessful);
			OnStartSessionCallback.Unbind();
		}
	}
}

bool USessionSubsystem::FindSessions(TSharedPtr<const FUniqueNetId> UserId, bool bIsLan, bool bIsPresence)
{
    IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();

    if(OnlineSub)
    {
        IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();

        if(Sessions.IsValid() && UserId.IsValid())
        {
            SessionSearch = MakeShareable(new FOnlineSessionSearch());

            SessionSearch->bIsLanQuery = bIsLan;
            SessionSearch->MaxSearchResults = 20;
            SessionSearch->PingBucketSize = 50;

            if(bIsPresence)
            {
                SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, bIsPresence, EOnlineComparisonOp::Equals);
            }

            TSharedRef<FOnlineSessionSearch> SearchSettingsRef = SessionSearch.ToSharedRef();

            OnFindSessionsCompleteDelegateHandle = Sessions->AddOnFindSessionsCompleteDelegate_Handle(OnFindSessionsCompleteDelegate);

            return Sessions->FindSessions(*UserId, SearchSettingsRef);
        }
        else
        {
            OnFindSessionsComplete(false);
        }
    }
	return false;
}

void USessionSubsystem::OnFindSessionsComplete(bool bWasSuccessful)
{
    IOnlineSubsystem* const OnlineSub = IOnlineSubsystem::Get();

    if(OnlineSub)
    {
        IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();

        if(Sessions.IsValid())
        {
            Sessions->ClearOnFindSessionsCompleteDelegate_Handle(OnFindSessionsCompleteDelegateHandle);
			OnFindSessionCallback.ExecuteIfBound(bWasSuccessful);
			OnFindSessionCallback.Unbind();
        }
    }
}

bool USessionSubsystem::JoinSession(TSharedPtr<const FUniqueNetId> UserId, FName SessionName, const FOnlineSessionSearchResult& SearchResult)
{
	bool bSuccessful = false;
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();

	if (OnlineSub)
	{
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();

		if (Sessions.IsValid() && UserId.IsValid())
		{
			OnJoinSessionCompleteDelegateHandle = Sessions->AddOnJoinSessionCompleteDelegate_Handle(OnJoinSessionCompleteDelegate);

			bSuccessful = Sessions->JoinSession(*UserId, SessionName, SearchResult);
		}
	}
		
	return bSuccessful;
}

void USessionSubsystem::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
	if (OnlineSub)
	{
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();

		if (Sessions.IsValid())
		{
			Sessions->ClearOnJoinSessionCompleteDelegate_Handle(OnJoinSessionCompleteDelegateHandle);

			APlayerController * const PlayerController = GetGameInstance()->GetFirstLocalPlayerController();

			FString TravelURL;

			OnJoinSessionCallback.ExecuteIfBound(true);
			OnJoinSessionCallback.Unbind();

			if (PlayerController && Sessions->GetResolvedConnectString(SessionName, TravelURL))
			{
				GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TravelURL);
				PlayerController->ClientTravel(TravelURL, ETravelType::TRAVEL_Absolute);
			}
		}
	}
}

void USessionSubsystem::OnDestroySessionComplete(FName SessionName, bool bWasSuccessful)
{
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();

	if (OnlineSub)
	{
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();

		if (Sessions.IsValid())
		{
			Sessions->ClearOnDestroySessionCompleteDelegate_Handle(OnDestroySessionCompleteDelegateHandle);

			if (OnDestroySessionCallback.IsBound())
			{
				OnDestroySessionCallback.ExecuteIfBound(bWasSuccessful);
				OnDestroySessionCallback.Unbind();
			}
			else if(bWasSuccessful)
			{
				UGameplayStatics::OpenLevel(GetWorld(), "Lobby", true);
			}
		}
	}
}

void USessionSubsystem::DestroySessionAndLeaveGame(FName SessionName)
{
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
	
	if (OnlineSub)
	{
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();

		if (Sessions.IsValid())
		{
			Sessions->AddOnDestroySessionCompleteDelegate_Handle(OnDestroySessionCompleteDelegate);

			Sessions->DestroySession(SessionName);
		}
	}
}

const TArray<FOnlineSessionSearchResult> USessionSubsystem::GetFoundSessions() const
{
	return SessionSearch->SearchResults;
}

void USessionSubsystem::Deinitialize()
{
	DestroySessionAndLeaveGame(NAME_GameSession);
}