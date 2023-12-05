// Fill out your copyright notice in the Description page of Project Settings.


#include "FPS/Weapon/Weapon.h"
#include "FPS/FPSCharacter.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SceneComponent.h"

AWeapon::AWeapon()
{
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);
}

void AWeapon::ExecuteLeft()
{
	if (LeftStrategy)
	{
		LeftStrategy->LeftAction();
	}
}

void AWeapon::ExecuteReload()
{
	if (ReloadStrategy)
	{
		ReloadStrategy->ReloadAction();
	}
}

void AWeapon::AttachToCharacter(AFPSCharacter* HitCharacter)
{
	Player = HitCharacter;
}
