// Copyright Epic Games, Inc. All Rights Reserved.

#include "Beetle_projectGameMode.h"
#include "Beetle_projectPlayerController.h"
#include "Beetle_projectCharacter.h"
#include "UObject/ConstructorHelpers.h"

ABeetle_projectGameMode::ABeetle_projectGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = ABeetle_projectPlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDown/Blueprints/BP_TopDownCharacter"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	// set default controller to our Blueprinted controller
	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerBPClass(TEXT("/Game/TopDown/Blueprints/BP_TopDownPlayerController"));
	if(PlayerControllerBPClass.Class != NULL)
	{
		PlayerControllerClass = PlayerControllerBPClass.Class;
	}
}