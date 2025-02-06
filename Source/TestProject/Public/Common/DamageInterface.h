// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "DamageInterface.generated.h"

struct FEffectStructure;

UINTERFACE(MinimalAPI)
class UDamageInterface : public UInterface
{
	GENERATED_BODY()
};

class TESTPROJECT_API IDamageInterface
{
	GENERATED_IINTERFACE_BODY()

	virtual void ReceiveEffect(const FEffectStructure& InEffectStructure);
};