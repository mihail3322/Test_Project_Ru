// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "EffectStructure.generated.h"

UENUM()
enum class EDurationTypeEffect : uint8
{
	InstantReduction,
	PeriodicReduction, 
	TemporaryReduction
};

UENUM()
enum class ETypeInfluenceEffect: uint8
{
	Health,
	Speed
};

USTRUCT(BlueprintType)
struct FEffectStructure : public FTableRowBase
{
	GENERATED_BODY()

	friend bool operator==(const FEffectStructure& A, const FEffectStructure& B)
	{
		return A.EffectID == B.EffectID;
	}

	UPROPERTY(EditAnywhere)
	EDurationTypeEffect DurationEffectTime;

	UPROPERTY(EditAnywhere)
	float Value{ 10.0f };

	/** It doesn't have any impact on InstantReduction  effect type */
	UPROPERTY(EditAnywhere, meta = (ClampMin = "0.5", EditCondition = "DurationEffectTime != EDurationTypeEffect::InstantReduction"))
	float TimeEffect{ 15.0f };

	UPROPERTY(EditAnywhere, meta = (EditCondition = "DurationEffectTime != EDurationTypeEffect::InstantReduction"))
	ETypeInfluenceEffect TypeInfluenceEffect;

	//We can use Asynchronous Asset Loading, but to avoid wasting time, I use common type.   
	UPROPERTY(EditAnywhere, meta = (EditCondition = "DurationEffectTime != EDurationTypeEffect::InstantReduction"))
	UTexture2D* EffectTexture;

	float AppliedTime;

	FName EffectID;
};