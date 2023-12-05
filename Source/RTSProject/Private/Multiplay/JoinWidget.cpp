// Fill out your copyright notice in the Description page of Project Settings.


#include "Multiplay/JoinWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"

void UJoinWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    JoinRoomName = Cast<UTextBlock>(GetWidgetFromName(TEXT("JoinRoomName")));
    JoinPlayerCount = Cast<UTextBlock>(GetWidgetFromName(TEXT("JoinPlayerCount")));
    JoinPlayer = Cast<UButton>(GetWidgetFromName(TEXT("JoinPlayer")));

    if(JoinPlayer)
    {
        JoinPlayer->OnClicked.AddDynamic(this, &UJoinWidget::OnClick_JoinPlayer);
    }
}

void UJoinWidget::OnClick_JoinPlayer()
{
    auto GameInstance = GetWorld()->GetGameInstance();
    auto Session = GameInstance->GetSubsystem<USessionSubsystem>();
    ULocalPlayer* const Player = GameInstance->GetFirstGamePlayer();
    Session->OnJoinSessionCallback.BindUObject(this, &UJoinWidget::OnJoin);
    
    Session->JoinSession(Player->GetPreferredUniqueNetId().GetUniqueNetId(), NAME_GameSession, SessionForJoin);
}

void UJoinWidget::SetSession(FOnlineSessionSearchResult FoundSession)
{
    SessionForJoin = FoundSession;
    auto Session = FoundSession.Session;
    auto SessionSetting = Session.SessionSettings;
    
    FString RoomNameFromSession;
    SessionSetting.Get<FString>(SettingName::SETTING_ROOMNAME, RoomNameFromSession);
    JoinRoomName->SetText(FText::FromString(RoomNameFromSession));
    JoinPlayerCount->SetText(FText::Format(FTextFormat::FromString("{0}/{1}"),SessionSetting.NumPublicConnections-Session.NumOpenPublicConnections+1 ,SessionSetting.NumPublicConnections+1));
}

void UJoinWidget::OnJoin(bool bWasSuccessful)
{
    if(bWasSuccessful)
    {
        if(OnJoinDelegate.IsBound())
        {
            OnJoinDelegate.Execute();
        }
    }
    else
    {
        GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Blue, TEXT("On Join Failed"));
    }
}