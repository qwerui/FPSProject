// Fill out your copyright notice in the Description page of Project Settings.


#include "Multiplay/RoomWidget.h"
#include "Multiplay/PlayerInfoWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Multiplay/SessionSubsystem.h"
#include "Multiplay/RoomController.h"
#include "Multiplay/RoomPlayerState.h"
#include "Components/VerticalBox.h"
#include "Multiplay/RoomGameState.h"

URoomWidget::URoomWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
    static ConstructorHelpers::FClassFinder<UPlayerInfoWidget> PlayerInfoAsset(TEXT("/Game/UI/Room/PlayerInfo.PlayerInfo_C"));

    if (PlayerInfoAsset.Succeeded())
    {
        PlayerInfoClass = PlayerInfoAsset.Class;
    }

    ConnectedPlayer = 0;
}

void URoomWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    BackButton = Cast<UButton>(GetWidgetFromName(TEXT("BackButton")));
    ConfirmButton = Cast<UButton>(GetWidgetFromName(TEXT("ConfirmButton")));
    RoomName = Cast<UTextBlock>(GetWidgetFromName(TEXT("RoomName")));
    PlayerInfoList = Cast <UVerticalBox>(GetWidgetFromName(TEXT("PlayerInfoList")));
    
    if(BackButton)
    {
        BackButton->OnClicked.AddDynamic(this, &URoomWidget::OnClick_BackButton);
    }

    if(ConfirmButton)
    {
        ConfirmButton->OnClicked.AddDynamic(this, &URoomWidget::OnClick_ConfirmButton);
    }
}

void URoomWidget::CreatePlayerInfo(int index, FString PlayerName, bool bIsLeader)
{
    if (index < PlayerInfoList->GetChildrenCount())
    {
        TObjectPtr<UPlayerInfoWidget> UpdatingChild = Cast<UPlayerInfoWidget>(PlayerInfoList->GetChildAt(index));
        UpdatingChild->SetVisibility(ESlateVisibility::Visible);
        UpdatingChild->Init(PlayerName, bIsLeader);
    }
    else
    {
        auto PlayerInfo = CreateWidget<UPlayerInfoWidget>(GetWorld(), PlayerInfoClass);
        PlayerInfo->Init(PlayerName, bIsLeader);

        PlayerInfoList->AddChildToVerticalBox(PlayerInfo);
        ConnectedPlayer++;
    }
}

void URoomWidget::RemovePlayerInfo(int PlayerCount)
{
    if (PlayerCount < ConnectedPlayer)
    {
        for(int i = PlayerCount; i < ConnectedPlayer; i++)
        {
            PlayerInfoList->GetChildAt(PlayerCount)->SetVisibility(ESlateVisibility::Hidden);
            ConnectedPlayer--;
        }
    }
}

void URoomWidget::SetReady(int PlayerIndex, bool bIsReady)
{
    if (PlayerIndex != 0)
    {
        Cast<UPlayerInfoWidget>(PlayerInfoList->GetChildAt(PlayerIndex))->CheckReady(bIsReady);
    }
}

void URoomWidget::OnClick_BackButton()
{
    TObjectPtr<USessionSubsystem> Session =  GetWorld()->GetGameInstance()->GetSubsystem<USessionSubsystem>();
    Session->DestroySessionAndLeaveGame(NAME_GameSession);

    GetWorld()->GetGameState<ARoomGameState>()->RequestPlayerLeave();
    UGameplayStatics::OpenLevel(GetWorld(), TEXT("Lobby"));
}

void URoomWidget::OnClick_ConfirmButton()
{
    auto Controller = GetOwningPlayer<ARoomController>();

    if (Controller->HasAuthority())
    {
        //Start Game (Server)
        GetWorld()->GetGameState<ARoomGameState>()->DivideTeam();
        Controller->StartGame();
    }
    else
    {
        //Ready (Client)
        Controller->Ready();
    }
}
