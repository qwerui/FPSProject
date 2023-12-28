// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FPS/Interface/LeftAction.h"
#include "FPS/Interface/ReloadAction.h"
#include "Weapon.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class RTSPROJECT_API AWeapon : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"));
	TObjectPtr<class USkeletalMeshComponent> Mesh;

	TWeakObjectPtr<class AFPSCharacter> Player;

	UPROPERTY()
	TScriptInterface<ILeftAction> LeftStrategy;
	UPROPERTY()
	TScriptInterface<IReloadAction> ReloadStrategy;

public:
	AWeapon();

	UPROPERTY(EditAnywhere, meta = (AllowedClasses = "/Script/Engine.Texture,/Script/Engine.MaterialInterface,/Script/Engine.SlateTextureAtlasInterface"))
	TObjectPtr<UObject> Icon;

	void ExecuteLeft();
	void ExecuteReload();

	virtual void OnReload() {}

	class AFPSCharacter* GetPlayer() const { return Player.IsValid() ? Player.Get() : nullptr; }
	virtual void AttachToCharacter(class AFPSCharacter* HitCharacter);
};
