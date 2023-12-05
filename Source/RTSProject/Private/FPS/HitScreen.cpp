// Fill out your copyright notice in the Description page of Project Settings.


#include "FPS/HitScreen.h"
#include "FPS/FPSWidget.h"

void UHitScreen::Release()
{
	SetVisibility(ESlateVisibility::Hidden);
	Parent->HitScreenPool.Enqueue(this);
}

void UHitScreen::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (CurrentDestroyTime >= DestroyTime)
	{
		Release();
	}

	CurrentDestroyTime += InDeltaTime;
}
