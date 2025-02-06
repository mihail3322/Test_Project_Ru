// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Common/DamageInterface.h"
#include "GameFramework/Character.h"
#include "Common/Data/EffectStructure.h"

#include "BaseBotCharacter.generated.h"

class UWidgetComponent;
class UHealthWidget;
class UEffectItemWidget;

UCLASS(Abstract)
class TESTPROJECT_API ABaseBotCharacter: public ACharacter, public IDamageInterface
{
	GENERATED_BODY()

	ABaseBotCharacter();
	virtual void BeginPlay() override;

	virtual void ReceiveEffect(const FEffectStructure& InEffectStructure) override;

	UFUNCTION()
	void ApplyPeriodicEffect(const FEffectStructure& EffectStructure);

	UFUNCTION()
	void RemoveAppliedEffect(const FEffectStructure& InEffectStructure);

	void CreateEffectWidget(const FEffectStructure& InEffectStructure);
	void RemoveEffectWidget(const FName& InEffectID);
	
	void TakeDamage(float InDamage);
	void TakeEffect(const FEffectStructure& InEffectStructure);

	UPROPERTY(EditAnywhere)
	UWidgetComponent* HealthWidgetComponent;
	
	UPROPERTY(EditAnywhere, Category = "Bot Settings")
	float Health {100.0f};

	UPROPERTY()
	UHealthWidget* HealthWidget;

	UPROPERTY()
	TMap<FName, UEffectItemWidget*> EffectWidgets;

	FTimerHandle PeriodicReductionTimer;
	FTimerHandle TemporaryReductionTimer;
	float MaxHealth;
};
