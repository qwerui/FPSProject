// Fill out your copyright notice in the Description page of Project Settings.


#include "FPS/FPSWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "FPS/Weapon/Gun.h"
#include "FPS/FPSCharacter.h"
#include "Slate/SlateTextureAtlasInterface.h"
#include "Components/CanvasPanel.h"
#include "FPS/FPSPlayerState.h"
#include "FPS/HitScreen.h"
#include "Components/Overlay.h"
#include "Components/OverlaySlot.h"

UFPSWidget::UFPSWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	static ConstructorHelpers::FClassFinder<UHitScreen> HitScreenAsset(TEXT("/Game/UI/FPS/BP_HitScreen.BP_HitScreen_C"));

	if (HitScreenAsset.Succeeded())
	{
		HitScreenClass = HitScreenAsset.Class;
	}
}

void UFPSWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	auto FPSCharacter = GetOwningPlayerPawn<AFPSCharacter>();

	if (FPSCharacter)
	{
		SetPlayer(FPSCharacter);
	}
}

void UFPSWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (DeadCanvas->GetVisibility() != ESlateVisibility::Hidden)
	{
		auto PlayerState = GetOwningPlayerState<AFPSPlayerState>();

		if (PlayerState)
		{
			RespawnTime->SetText(FText::FromString(FString::Printf(TEXT("Respawn Time : %d"), (int)PlayerState->GetRespawnTime())));
		}
	}
}

void UFPSWidget::AmmoUpdate()
{
	if (CurrentGun.IsValid())
	{
		AmmoText->SetText(FText::FromString(CurrentGun->GetAmmoStateString()));
	}
}

void UFPSWidget::HpUpdate()
{
	if (PlayerCharacter.IsValid())
	{
		HPText->SetText(FText::FromString(FString::Printf(TEXT("%d"), (int)PlayerCharacter->GetHP())));
	}
}

void UFPSWidget::SetPlayer(AFPSCharacter* NewCharacter)
{
	if (NewCharacter)
	{
		PlayerCharacter = NewCharacter;
		PlayerCharacter->OnHPChangedEvent.AddUObject(this, &UFPSWidget::HpUpdate);
		HpUpdate();
	}
}

void UFPSWidget::SetGun(AGun* NewGun)
{
	if (NewGun)
	{
		CurrentGun = NewGun;

		CurrentWeaponIcon->SetVisibility(ESlateVisibility::Visible);
		AmmoText->SetVisibility(ESlateVisibility::Visible);

		auto Icon = CurrentGun->Icon;

		if (Icon)
		{
			FSlateBrush NewIconBrush;
			auto Sprite = Cast<ISlateTextureAtlasInterface>(Icon);

			if (Sprite)
			{
				FVector2D SpriteSize = Sprite->GetSlateAtlasData().SizeUV;
				NewIconBrush.SetImageSize(SpriteSize);
			}
			
			NewIconBrush.SetResourceObject(Icon);
			CurrentWeaponIcon->SetBrush(NewIconBrush);
		}

		CurrentGun->OnReloadEvent.AddUObject(this, &UFPSWidget::AmmoUpdate);
		CurrentGun->OnShotEvent.AddUObject(this, &UFPSWidget::AmmoUpdate);
		AmmoUpdate();
	}	
}

void UFPSWidget::SetDeadWidgetVisibility(bool bIsEnable)
{
	if (bIsEnable)
	{
		DeadCanvas->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		CurrentWeaponIcon->SetVisibility(ESlateVisibility::Hidden);
		AmmoText->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		DeadCanvas->SetVisibility(ESlateVisibility::Hidden);
	}
	
}

void UFPSWidget::SetGameEndVisibility(bool bIsRedWin)
{
	if (bIsRedWin)
	{
		WinnerTeam->SetText(FText::FromString(TEXT("Red Team Wins")));
	}
	else
	{
		WinnerTeam->SetText(FText::FromString(TEXT("Blue Team Wins")));
	}

	GameEndCanvas->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}

void UFPSWidget::EnableHitScreen(float Angle)
{
	TObjectPtr<UHitScreen> HitScreen;

	if (HitScreenPool.IsEmpty())
	{
		if (HitScreenClass)
		{
			HitScreen = CreateWidget<UHitScreen>(this, HitScreenClass);
			HitScreen->SetParent(this);
			auto Attached = HitScreenPosition->AddChildToOverlay(HitScreen);
			Attached->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Fill);
			Attached->SetVerticalAlignment(EVerticalAlignment::VAlign_Fill);
		}
	}
	else
	{
		HitScreenPool.Dequeue(HitScreen);
		HitScreen->Reset();
	}
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Blue, FString::Printf(TEXT("%f"), Angle));
	HitScreen->SetRenderTransformAngle(Angle);
	HitScreen->SetVisibility(ESlateVisibility::Visible);
}
