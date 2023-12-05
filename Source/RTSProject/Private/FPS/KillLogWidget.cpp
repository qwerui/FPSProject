// Fill out your copyright notice in the Description page of Project Settings.


#include "FPS/KillLogWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"

void UKillLogWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (CurrentReleaseTime >= ReleaseTime)
	{
		Release();
	}

	CurrentReleaseTime += InDeltaTime;
}

void UKillLogWidget::InitWidget(FString Kill, FString Dead, FSlateBrush IconBrush, bool bKillIsRed)
{
	KillName->SetText(FText::FromString(Kill));
	DeadName->SetText(FText::FromString(Dead));
	WeaponIcon->SetBrush(IconBrush);

	if (bKillIsRed)
	{
		Background->GetDynamicMaterial()->SetScalarParameterValue(FName("IsRedKill"), 1);
	}
	else
	{
		Background->GetDynamicMaterial()->SetScalarParameterValue(FName("IsRedKill"), 0);
	}
}

void UKillLogWidget::Release()
{
	RemoveFromParent();
}
