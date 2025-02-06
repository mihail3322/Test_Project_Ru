// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/BaseCharacter.h"

#include "EnhancedInputComponent.h"
#include "EnhancedPlayerInput.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SlateWrapperTypes.h"
#include "Weapons/BaseWeaponComponent.h"


ABaseCharacter::ABaseCharacter()
{
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	CameraComponent->SetupAttachment(GetCapsuleComponent());
	CameraComponent->bUsePawnControlRotation = true;

	HandsMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	HandsMesh->SetOnlyOwnerSee(true);
	HandsMesh->SetupAttachment(CameraComponent);
	HandsMesh->bCastDynamicShadow = false;
	HandsMesh->CastShadow = false;

	WeaponComponent0 = CreateDefaultSubobject<UBaseWeaponComponent>(TEXT("Weapon0"));
	WeaponComponent1 = CreateDefaultSubobject<UBaseWeaponComponent>(TEXT("Weapon1"));
	WeaponComponent2 = CreateDefaultSubobject<UBaseWeaponComponent>(TEXT("Weapon2"));

	WeaponComponent0->SetupAttachment(HandsMesh, "GripPoint");
	WeaponComponent1->SetupAttachment(HandsMesh, "GripPoint");
	WeaponComponent2->SetupAttachment(HandsMesh, "GripPoint");
}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ThisClass::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ThisClass::Look);
		EnhancedInputComponent->BindAction(FireFromWeapon0, ETriggerEvent::Started, this, &ThisClass::UseWeapon<0>);
		EnhancedInputComponent->BindAction(FireFromWeapon1, ETriggerEvent::Started, this, &ThisClass::UseWeapon<1>);
		EnhancedInputComponent->BindAction(FireFromWeapon2, ETriggerEvent::Started, this, &ThisClass::UseWeapon<2>);
	}
}

void ABaseCharacter::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

void ABaseCharacter::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

template <uint8 IndexWeapon>
void ABaseCharacter::UseWeapon()
{
	// Hide old weapon and show new one
	GetWeaponByIndex(CurrentIndexWeapon)->SetVisibility(false);
	GetWeaponByIndex(IndexWeapon)->SetVisibility(true);

	GetWeaponByIndex(IndexWeapon)->FireFromWeapon();
	CurrentIndexWeapon = IndexWeapon;
}

UBaseWeaponComponent* ABaseCharacter::GetWeaponByIndex(int32 IndexWeapon) const
{
	switch (IndexWeapon)
	{
		case 0 : return WeaponComponent0;
		case 1 : return WeaponComponent1;
		case 2 : return WeaponComponent2;
		default: return WeaponComponent0;
	}
}