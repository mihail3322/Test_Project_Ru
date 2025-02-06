// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EffectItemWidget.generated.h"

class UImage;

UCLASS()
class TESTPROJECT_API UEffectItemWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta=(BindWidget))
	UImage* EffectImage;
};
