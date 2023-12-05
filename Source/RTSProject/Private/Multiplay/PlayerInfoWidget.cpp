// Fill out your copyright notice in the Description page of Project Settings.


#include "Multiplay/PlayerInfoWidget.h"
#include "Slate/SlateBrushAsset.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

UPlayerInfoWidget::UPlayerInfoWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	static ConstructorHelpers::FObjectFinder<USlateBrushAsset> LeaderAsset(TEXT("/Game/UI/Room/Sprite/Leader_Brush.Leader_Brush"));

	if (LeaderAsset.Succeeded())
	{
		LeaderSprite = LeaderAsset.Object;
	}

	static ConstructorHelpers::FObjectFinder<USlateBrushAsset> ReadyAsset(TEXT("/Game/UI/Room/Sprite/Check_Brush.Check_Brush"));
	
	if (ReadyAsset.Succeeded())
	{
		ReadySprite = ReadyAsset.Object;
	}
}

void UPlayerInfoWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	ReadyCheckImage = Cast<UImage>(GetWidgetFromName(TEXT("ReadyCheck")));
	PlayerNameText = Cast<UTextBlock>(GetWidgetFromName(TEXT("PlayerName")));
}

void UPlayerInfoWidget::Init(FString PlayerName, bool bIsLeader)
{
	PlayerNameText->SetText(FText::FromString(PlayerName));

	if (bIsLeader)
	{
		ReadyCheckImage->SetBrushFromAsset(LeaderSprite);
		ReadyCheckImage->SetOpacity(1.0f);
	}
	else
	{
		ReadyCheckImage->SetBrushFromAsset(ReadySprite);
		ReadyCheckImage->SetOpacity(0.0f);
	}
}

void UPlayerInfoWidget::CheckReady(bool bIsReady)
{
	if (bIsReady)
	{
		ReadyCheckImage->SetOpacity(1.0f);
	}
	else
	{
		ReadyCheckImage->SetOpacity(0.0f);
	}
}

