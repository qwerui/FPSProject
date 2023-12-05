// Fill out your copyright notice in the Description page of Project Settings.


#include "FPS/Strategy/LeftFire.h"
#include "FPS/FPSCharacter.h"
#include "FPS/Weapon/Gun.h"

void ULeftFire::LeftAction()
{
	if (OwnerGun != nullptr)
	{
		if (OwnerGun->CanShot() && !OwnerGun->bIsReloading)
		{
			auto Player = OwnerGun->GetPlayer();
			
			if (Player)
			{
				Player->CheckShotRaycast(OwnerGun->GetDamage());
			}

			OwnerGun->Shot();
		}
	}
}

void ULeftFire::SetGun(AGun* NewGun)
{
	OwnerGun = NewGun;
}
