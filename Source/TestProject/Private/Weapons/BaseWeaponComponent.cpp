// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/BaseWeaponComponent.h"

#include "Weapons/BaseWeaponProjectile.h"

void UBaseWeaponComponent::FireFromWeapon()
{
	if(ProjectileClass)
	{
		FTransform SpawnTransform;
		SpawnTransform.SetRotation(FQuat(GetWorld()->GetFirstPlayerController()->PlayerCameraManager->GetCameraRotation()));
		SpawnTransform.SetLocation(GetOwner()->GetActorLocation() + SpawnTransform.GetRotation().RotateVector(FVector(100.0f, 0.0f, 40.0f)));

		ABaseWeaponProjectile* WeaponProjectile = GetWorld()->SpawnActorDeferred<ABaseWeaponProjectile>(ProjectileClass, SpawnTransform, GetOwner());

		if(!WeaponProjectile)
		{
			UE_LOG(LogTemp, Error, TEXT("Faild to spawn projectile"))
			return;
		}

		WeaponProjectile->SetEffectStructure(WeaponEffect);

		WeaponProjectile->FinishSpawning(SpawnTransform);
	}
}

void UBaseWeaponComponent::BeginPlay()
{
	Super::BeginPlay();

	const FEffectStructure* DataTableEffect = EffectDataTable->FindRow<FEffectStructure>(EffectName, "");

	if(DataTableEffect)
	{
		WeaponEffect = *DataTableEffect;
		WeaponEffect.EffectID = EffectName;
	}
}
