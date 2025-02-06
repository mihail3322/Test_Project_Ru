// Fill out your copyright notice in the Description page of Project Settings.


#include "Bot/BaseBotCharacter.h"

#include "Bot/UI/EffectItemWidget.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Bot/UI/HealthWidget.h"
#include "Components/HorizontalBox.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

ABaseBotCharacter::ABaseBotCharacter()
{
	HealthWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("HeathWidgetComponent"));
	HealthWidgetComponent->SetupAttachment(GetRootComponent());

	MaxHealth = Health;
}

void ABaseBotCharacter::BeginPlay()
{
	Super::BeginPlay();

	HealthWidget = Cast<UHealthWidget>(HealthWidgetComponent->GetWidget());
	HealthWidget->HealthTextBlock->SetText(FText::FromString(FString::Printf(TEXT("%i/%i"), static_cast<int32>(Health), static_cast<int32>(MaxHealth))));
}

void ABaseBotCharacter::ReceiveEffect(const FEffectStructure& InEffectStructure)
{
	// Check on Instant Reduction
	if(InEffectStructure.DurationEffectTime == EDurationTypeEffect::InstantReduction)
	{
		if(InEffectStructure.TypeInfluenceEffect == ETypeInfluenceEffect::Health)
		{
			TakeDamage(InEffectStructure.Value);
		}

		return; 
	}

	FEffectStructure LocalStructure = InEffectStructure;
	LocalStructure.AppliedTime = GetWorld()->GetTimeSeconds();

	if(InEffectStructure.DurationEffectTime == EDurationTypeEffect::PeriodicReduction)
	{
		constexpr  float TimeRateTemporaryEffects = 1.0f;

		GetWorld()->GetTimerManager().SetTimer(PeriodicReductionTimer,
			[this, LocalStructure]()
						{ApplyPeriodicEffect(LocalStructure);},
						TimeRateTemporaryEffects,
						true);
	}
	else
	{
		//Clear old effect if we already had it
		if(TemporaryReductionTimer.IsValid())
		{
			GetWorld()->GetTimerManager().ClearTimer(TemporaryReductionTimer);
			RemoveAppliedEffect(InEffectStructure);
		}

		GetWorld()->GetTimerManager().SetTimer(TemporaryReductionTimer,
			[this, InEffectStructure]()
						{RemoveAppliedEffect(InEffectStructure);},
						InEffectStructure.TimeEffect,
						false);
	}

	TakeEffect(InEffectStructure);
	CreateEffectWidget(InEffectStructure);
}

void ABaseBotCharacter::ApplyPeriodicEffect(const FEffectStructure& EffectStructure)
{
	if (GetWorld()->GetTimeSeconds() - EffectStructure.AppliedTime > EffectStructure.TimeEffect)
	{
		RemoveEffectWidget(EffectStructure.EffectID);
		GetWorld()->GetTimerManager().ClearTimer(PeriodicReductionTimer);
	}
	else
	{
		TakeEffect(EffectStructure);
	}
}

void ABaseBotCharacter::CreateEffectWidget(const FEffectStructure& InEffectStructure)
{
	if(EffectWidgets.Contains(InEffectStructure.EffectID))
	{
		return;
	}

	UEffectItemWidget* WidgetEffect = CreateWidget<UEffectItemWidget>(HealthWidget, HealthWidget->EffectItemWidgetClass);

	if(WidgetEffect)
	{
		WidgetEffect->EffectImage->SetBrushFromTexture(InEffectStructure.EffectTexture);

		HealthWidget->EffectsHorizontalBox->AddChild(WidgetEffect);
	}

	EffectWidgets.Emplace(InEffectStructure.EffectID, WidgetEffect);
}

void ABaseBotCharacter::RemoveEffectWidget(const FName& InEffectID)
{
	if(EffectWidgets.Contains(InEffectID))
	{
		(*EffectWidgets.Find(InEffectID))->RemoveFromParent();
		EffectWidgets.Remove(InEffectID);
	}
}

void ABaseBotCharacter::TakeDamage(float InDamage)
{
	Health -= InDamage;

	if(Health <= 0)
	{
		Destroy();

		return;
	}

	HealthWidget->HealthProgressBar->SetPercent(Health/MaxHealth);
	HealthWidget->HealthTextBlock->SetText(FText::FromString(FString::Printf(TEXT("%i/%i"), static_cast<int32>(Health), static_cast<int32>(MaxHealth))));
}

void ABaseBotCharacter::TakeEffect(const FEffectStructure& InEffectStructure)
{
	switch (InEffectStructure.TypeInfluenceEffect)
	{
		case ETypeInfluenceEffect::Health:
		{
			TakeDamage(InEffectStructure.Value);
		}
		case ETypeInfluenceEffect::Speed:
		{
			GetCharacterMovement()->MaxWalkSpeed -= InEffectStructure.Value;
		}
	}
}

void ABaseBotCharacter::RemoveAppliedEffect(const FEffectStructure& InEffectStructure)
{
	switch (InEffectStructure.TypeInfluenceEffect)
	{
		case ETypeInfluenceEffect::Health:
		{
			TakeDamage(InEffectStructure.Value * -1);
		}
		case ETypeInfluenceEffect::Speed:
		{
			GetCharacterMovement()->MaxWalkSpeed += InEffectStructure.Value;
		}
	}

	RemoveEffectWidget(InEffectStructure.EffectID);
}
