// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "Multiplay/LobbyWidget.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Components/EditableTextBox.h"
#include "Kismet/GameplayStatics.h"
#include "Components/TextBlock.h"
#include "System/LocalDataSubsystem.h"
#include "Multiplay/SessionSubsystem.h"
#include "Online/CoreOnline.h"
#include "Multiplay/JoinWidget.h"
#include "Components/VerticalBox.h"
#include "System/PlayerSave.h"

ULobbyWidget::ULobbyWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
    static ConstructorHelpers::FClassFinder<UJoinWidget> JoinWidgetAsset(TEXT("/Game/UI/Lobby/Join.Join_C"));

    if(JoinWidgetAsset.Succeeded())
    {
        JoinWidgetClass = JoinWidgetAsset.Class;
    }
}

void ULobbyWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    Switcher = Cast<UWidgetSwitcher>(GetWidgetFromName(TEXT("Switcher")));
    PlayerName = Cast<UTextBlock>(GetWidgetFromName(TEXT("PlayerName")));

    auto GameInstance = GetWorld()->GetGameInstance();
    auto Session = GameInstance->GetSubsystem<USessionSubsystem>();
    ULocalDataSubsystem* LocalData = GameInstance->GetSubsystem<ULocalDataSubsystem>();

    auto LoadGameInstance = Cast<UPlayerSave>(UGameplayStatics::CreateSaveGameObject(UPlayerSave::StaticClass()));
    LoadGameInstance = Cast<UPlayerSave>(UGameplayStatics::LoadGameFromSlot(LoadGameInstance->SaveSlotName, LoadGameInstance->UserIndex));

    if (LoadGameInstance)
    {
        PlayerName->SetText(FText::FromString(FString::Printf(TEXT("Player Name : %s"), *LoadGameInstance->PlayerName)));
        LocalData->SetPlayerName(LoadGameInstance->PlayerName);
    }
    else
    {
        PlayerName->SetText(FText::FromString(TEXT("Player Name : Default Name")));
        LocalData->SetPlayerName(TEXT("Default Name"));
    }

#pragma region TitleCanvas
    HostButton = Cast<UButton>(GetWidgetFromName(TEXT("HostButton")));
    JoinButton = Cast<UButton>(GetWidgetFromName(TEXT("JoinButton")));

    if(HostButton)
    {
        HostButton->OnClicked.AddDynamic(this, &ULobbyWidget::OnClick_HostButton);
    }

    if(JoinButton)
    {
        JoinButton->OnClicked.AddDynamic(this, &ULobbyWidget::OnClick_JoinButton);
    }
#pragma endregion

#pragma region JoinCanvas

    BackButton = Cast<UButton>(GetWidgetFromName(TEXT("BackButton")));
    RoomList = Cast<UVerticalBox>(GetWidgetFromName(TEXT("RoomList")));
    JoinSwitcher = Cast<UWidgetSwitcher>(GetWidgetFromName(TEXT("JoinSwitcher")));

    if(BackButton)
    {
        BackButton->OnClicked.AddDynamic(this, &ULobbyWidget::OnClick_BackButton);
    }

#pragma endregion

#pragma region CreateCanvas

    CreateButton = Cast<UButton>(GetWidgetFromName(TEXT("CreateButton")));
    CancelButton = Cast<UButton>(GetWidgetFromName(TEXT("CancelButton")));
    RoomName = Cast<UEditableTextBox>(GetWidgetFromName(TEXT("RoomName")));

    if(CreateButton)
    {
        CreateButton->OnClicked.AddDynamic(this, &ULobbyWidget::OnClick_CreateButton);
    }

    if(CancelButton)
    {
        CancelButton->OnClicked.AddDynamic(this, &ULobbyWidget::OnClick_BackButton);
    }

#pragma endregion

#pragma region NameChange

    NameButton = Cast<UButton>(GetWidgetFromName(TEXT("NameButton")));
    NameConfirmButton = Cast<UButton>(GetWidgetFromName(TEXT("NameConfirmButton")));
    NameCancelButton = Cast<UButton>(GetWidgetFromName(TEXT("NameCancelButton")));
    NewNameBox = Cast<UEditableTextBox>(GetWidgetFromName(TEXT("NewNameBox")));
    CurrentName = Cast<UTextBlock>(GetWidgetFromName(TEXT("CurrentName")));

    if (NameButton)
    {
        NameButton->OnClicked.AddDynamic(this, &ULobbyWidget::OnClick_NameButton);
    }

    if (NameConfirmButton)
    {
        NameConfirmButton->OnClicked.AddDynamic(this, &ULobbyWidget::OnClick_NameConfirmButton);
    }

    if (NameCancelButton)
    {
        NameCancelButton->OnClicked.AddDynamic(this, &ULobbyWidget::OnClick_BackButton);
    }
    
#pragma endregion
}

void ULobbyWidget::OnClick_HostButton()
{
    if(Switcher)
    {
        Switcher->SetActiveWidgetIndex(2);
        RoomName->SetText(FText::FromString(""));
    }
}

void ULobbyWidget::OnClick_JoinButton()
{
    if(Switcher)
    {
        Switcher->SetActiveWidgetIndex(1);
        auto GameInstance = GetWorld()->GetGameInstance();
        auto Session = GameInstance->GetSubsystem<USessionSubsystem>();
        ULocalPlayer* const Player = GameInstance->GetFirstGamePlayer();

        Session->OnFindSessionCallback.BindUObject(this, &ULobbyWidget::FindSessionCompleted);

        if(Session->FindSessions(Player->GetPreferredUniqueNetId().GetUniqueNetId(), false, false))
        {
            
        }
        else
        {
            
        }
    }
}

void ULobbyWidget::OnClick_BackButton()
{
    if(Switcher)
    {
        RoomList->ClearChildren();
        JoinSwitcher->SetActiveWidgetIndex(0);
        Switcher->SetActiveWidgetIndex(0);
    }
}
void ULobbyWidget::OnClick_CreateButton()
{
    if(RoomName)
    {
        if(!RoomName->GetText().IsEmptyOrWhitespace())
        {
            auto LocalData = GetWorld()->GetGameInstance()->GetSubsystem<ULocalDataSubsystem>();
            LocalData->SetRoomName(RoomName->GetText());
            UGameplayStatics::OpenLevel(GetWorld(), TEXT("Room"), true, (FString)(L"Listen"));
        }
    }
}

void ULobbyWidget::OnClick_NameButton()
{
    if (Switcher)
    {
        Switcher->SetActiveWidgetIndex(3);
        UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(GetWorld());
        ULocalDataSubsystem* LocalData = GameInstance->GetSubsystem<ULocalDataSubsystem>();
        CurrentName->SetText(FText::FromString(FString::Printf(TEXT("Current Name : %s"), *LocalData->GetPlayerName())));
        NewNameBox->SetText(FText::FromString(""));
    }
}

void ULobbyWidget::OnClick_NameConfirmButton()
{
    if (Switcher)
    {
        auto SaveGameInstance = Cast<UPlayerSave>(UGameplayStatics::CreateSaveGameObject(UPlayerSave::StaticClass()));
        SaveGameInstance->PlayerName = NewNameBox->GetText().ToString();
        UGameplayStatics::SaveGameToSlot(SaveGameInstance, SaveGameInstance->SaveSlotName, SaveGameInstance->UserIndex);

        UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(GetWorld());
        ULocalDataSubsystem* LocalData = GameInstance->GetSubsystem<ULocalDataSubsystem>();
        LocalData->SetPlayerName(NewNameBox->GetText().ToString());

        PlayerName->SetText(FText::FromString(FString::Printf(TEXT("Player Name : %s"), *NewNameBox->GetText().ToString())));
        Switcher->SetActiveWidgetIndex(0);
    }
}

void ULobbyWidget::FindSessionCompleted(bool bWasSuccessful)
{
    if(bWasSuccessful)
    {
        JoinSwitcher->SetActiveWidgetIndex(1);

        auto Session = GetWorld()->GetGameInstance()->GetSubsystem<USessionSubsystem>();

        for(auto FoundSession : Session->GetFoundSessions())
        {
            UJoinWidget* CreatedJoin = CreateWidget<UJoinWidget>(GetWorld(), JoinWidgetClass);
            CreatedJoin->SetSession(FoundSession);
            CreatedJoin->OnJoinDelegate.BindUObject(this, &ULobbyWidget::NowJoining);
            RoomList->AddChildToVerticalBox(CreatedJoin);
        }
    }
    else
    {

    }
}

void ULobbyWidget::NowJoining()
{
    if(JoinSwitcher)
    {
        JoinSwitcher->SetActiveWidgetIndex(2);
    }   
}
