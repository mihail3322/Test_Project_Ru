// Copyright Epic Games, Inc. All Rights Reserved.

#include "Weapons/BaseWeaponProjectile.h"

#include "Common/DamageInterface.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"

ABaseWeaponProjectile::ABaseWeaponProjectile() 
{
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComp->OnComponentHit.AddDynamic(this, &ABaseWeaponProjectile::OnHit);

	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	RootComponent = CollisionComp;

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;

	InitialLifeSpan = 3.0f;
}

void ABaseWeaponProjectile::SetEffectStructure(const FEffectStructure& InEffectStructure)
{
	ProjectileEffectStructure = InEffectStructure;
}

void ABaseWeaponProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	const auto DamagedACtor = Cast<IDamageInterface>(OtherActor);

	if (DamagedACtor)
	{
		DamagedACtor->ReceiveEffect(ProjectileEffectStructure);

		Destroy();
	}
}
