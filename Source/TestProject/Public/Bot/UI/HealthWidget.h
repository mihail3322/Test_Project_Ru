// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "HealthWidget.generated.h"

class UProgressBar;
class UTextBlock;
class UHorizontalBox;
class UEffectItemWidget;

UCLASS()
class TESTPROJECT_API UHealthWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta=(BindWidget))
	UProgressBar* HealthProgressBar;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* HealthTextBlock;

	UPROPERTY(meta=(BindWidget))
	UHorizontalBox* EffectsHorizontalBox;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UEffectItemWidget> EffectItemWidgetClass;
};
