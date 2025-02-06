// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SkeletalMeshComponent.h"
#include "Common/Data/EffectStructure.h"

#include "BaseWeaponComponent.generated.h"

class ABaseWeaponProjectile;

UCLASS()
class TESTPROJECT_API UBaseWeaponComponent : public USkeletalMeshComponent
{
	GENERATED_BODY()

public:
	void FireFromWeapon();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category= "Weapon Settings")
	UDataTable* EffectDataTable;

	UPROPERTY(EditAnywhere, Category = "Weapon Settings")
	TSubclassOf<ABaseWeaponProjectile> ProjectileClass;

	UPROPERTY(EditAnywhere, Category = "Weapon Settings")
	FName EffectName;

	FEffectStructure WeaponEffect;
};
