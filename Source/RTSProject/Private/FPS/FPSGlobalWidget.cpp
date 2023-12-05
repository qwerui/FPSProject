// Fill out your copyright notice in the Description page of Project Settings.


#include "FPS/FPSGlobalWidget.h"
#include "FPS/KillLogWidget.h"
#include "Slate/SlateTextureAtlasInterface.h"
#include "Components/VerticalBox.h"
#include "Components/RichTextBlock.h"

UFPSGlobalWidget::UFPSGlobalWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	static ConstructorHelpers::FClassFinder<UKillLogWidget> KillLogAsset(TEXT("/Game/UI/FPS/BP_KillLog.BP_KillLog_C"));

	if (KillLogAsset.Succeeded())
	{
		KillLogClass = KillLogAsset.Class;
	}
}

void UFPSGlobalWidget::UpdateGlobalScore(int8 RedKill, int8 BlueKill)
{
	GlobalScore->SetText(FText::FromString(FString::Printf(TEXT("<RedTeam>%d</> <Base>:</> <BlueTeam>%d</>"), RedKill, BlueKill)));
}

void UFPSGlobalWidget::ShowKillLog(FString DeadName, FString KillName, UObject* WeaponIcon, bool bKillIsRed)
{
	TObjectPtr<UKillLogWidget> TargetWidget = nullptr;

	if (KillLogPool.IsEmpty())
	{
		TargetWidget = CreateWidget<UKillLogWidget>(this, KillLogClass);
		TargetWidget->Parent = this;
	}
	else
	{
		KillLogPool.Dequeue(TargetWidget);
	}

	FSlateBrush WeaponBrush;
	
	if (WeaponIcon)
	{
		
		auto Sprite = Cast<ISlateTextureAtlasInterface>(WeaponIcon);

		if (Sprite)
		{
			FVector2D SpriteSize = Sprite->GetSlateAtlasData().SizeUV;
			WeaponBrush.SetImageSize(SpriteSize);
		}

		WeaponBrush.SetResourceObject(WeaponIcon);
	}
	
	TargetWidget->InitWidget(DeadName, KillName, WeaponBrush, bKillIsRed);
	KillLogBox->AddChildToVerticalBox(TargetWidget);
}

void UFPSGlobalWidget::OnRelease(UKillLogWidget* ReleasedWidget)
{
	KillLogPool.Enqueue(ReleasedWidget);
}
