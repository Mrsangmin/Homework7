// Fill out your copyright notice in the Description page of Project Settings.


#include "SpartaGameMode.h"
#include "MyPawn.h"
#include "SpartaPlayerController.h"

ASpartaGameMode::ASpartaGameMode()
{
	DefaultPawnClass = AMyPawn::StaticClass();
	PlayerControllerClass = ASpartaPlayerController::StaticClass();
}