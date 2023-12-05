// Fill out your copyright notice in the Description page of Project Settings.


#include "FPS/Weapon/Rifle.h"
#include "FPS/Strategy/LeftFire.h"
#include "FPS/Strategy/Reloading.h"

ARifle::ARifle()
{
	
}

void ARifle::BeginPlay()
{
	Super::BeginPlay();

	auto Left = NewObject<ULeftFire>(this);
	Left->SetGun(this);
	LeftStrategy = Left;

	auto Reload = NewObject<UReloading>(this);
	Reload->SetGun(this);
	ReloadStrategy = Reload;
}
