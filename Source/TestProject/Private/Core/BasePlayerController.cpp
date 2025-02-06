// Copyright Epic Games, Inc. All Rights Reserved.


#include "Core/BasePlayerController.h"

#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"

void ABasePlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(InputMappingContext, 0);
	}
}