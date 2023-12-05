// Fill out your copyright notice in the Description page of Project Settings.


#include "FPS/Strategy/Reloading.h"
#include "FPS/FPSCharacter.h"
#include "FPS/Weapon/Gun.h"

void UReloading::ReloadAction()
{
	auto Player = Gun->GetPlayer();

	if (Player && Gun)
	{
		if (Gun->FirstReloadAnim && Gun->CanReload())
		{
			Player->PlaySound(Gun->ReloadSound);
			Gun->bIsReloading = true;
			Player->PlayAnimMontageAtFirstMesh(Gun->FirstReloadAnim, 1.0f);
			Player->SendReloadToServer();
		}
	}
}