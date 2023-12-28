// Fill out your copyright notice in the Description page of Project Settings.


#include "FPS/Weapon/Gun.h"
#include "FPS/FPSCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraComponent.h"
#include "FPS/FPSWidget.h"
#include "FPS/FPSPlayerController.h"
#include "Engine/DecalActor.h"
#include "FPS/Weapon/Crosshair.h"

AGun::AGun()
{
	MuzzleFlash = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Muzzle Flash"));
	MuzzleFlash->SetupAttachment(RootComponent);

	PrimaryActorTick.bCanEverTick = true;
}

void AGun::AttachToCharacter(AFPSCharacter* Character)
{
	Super::AttachToCharacter(Character);

	auto Controller = Character->GetController<AFPSPlayerController>();

	if (Controller)
	{
		auto Widget = Controller->GetWidget();

		if (Widget)
		{
			Widget->SetGun(this);
		}

		if (CrosshairClass && Controller == GetWorld()->GetFirstPlayerController())
		{
			Crosshair = CreateWidget<UCrosshair>(GetWorld(), CrosshairClass);
			Crosshair->AddToViewport();
		}
		
	}

	Character->SetGun(this);
}

void AGun::BeginPlay()
{
	Super::BeginPlay();

	CurrentAmmo = MaxAmmo;
}

void AGun::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	CurrentShotDelay -= DeltaSeconds;
}

void AGun::Destroyed()
{
	if (Crosshair)
	{
		Crosshair->RemoveFromParent();
	}

	Super::Destroyed();
}

void AGun::Shot()
{
	CurrentShotDelay = ShotDelay;
	CurrentAmmo--;

	MuzzleFlash->ActivateSystem();

	if (Crosshair)
	{
		Crosshair->PlayShotAnimation();
	}

	if (OnShotEvent.IsBound())
	{
		OnShotEvent.Broadcast();
	}

	if (Player.IsValid())
	{
		FHitResult Hit = Player->CheckRaycastHit();

		if (Hit.bBlockingHit)
		{
			if (Hit.GetActor()->ActorHasTag(FName(TEXT("Background"))))
			{
				GetWorld()->SpawnActor<ADecalActor>(BulletMarkClass, Hit.Location, Hit.Normal.Rotation() + FRotator(-90, 0, 0));
			}
		}

		Player->PlaySound(FireSound);
	}
}

void AGun::OnReload()
{
	CurrentAmmo = MaxAmmo;
	bIsReloading = false;

	if (OnReloadEvent.IsBound())
	{
		OnReloadEvent.Broadcast();
	}
}
