// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerController.h"
#include "Project2/UI/MyMatchTimerWidget.h"
#include "Project2/UI/MyMatchEndWidget.h"

AMyPlayerController::AMyPlayerController()
{
	static ConstructorHelpers::FClassFinder<UMyMatchTimerWidget> MyMatchTimerWidgetRef(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Project2/UI/WBP_MatchTimer.WBP_MatchTimer_C'"));
	if (MyMatchTimerWidgetRef.Class)
	{
		MyTimerWidgetClass = MyMatchTimerWidgetRef.Class;
	}

	static ConstructorHelpers::FClassFinder<UMyMatchEndWidget> MyMatchEndWidgetRef(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Project2/UI/WBP_MatchEnd.WBP_MatchEnd_C'"));
	if (MyMatchEndWidgetRef.Class)
	{
		MyMatchEndWidgetClass = MyMatchEndWidgetRef.Class;
	}

}

void AMyPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	if (IsLocalController())
	{
		MyTimerWidget = CreateWidget(GetWorld(), MyTimerWidgetClass);
		if (MyTimerWidget)
		{
			MyTimerWidget->AddToViewport();
		}

		MyMatchEndWidget = CreateWidget(GetWorld(), MyMatchEndWidgetClass);
		if (MyMatchEndWidget)
		{
			MyMatchEndWidget->AddToViewport();
		}

		MyMatchEndWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
	
	// Control directly turn to game input when play start
	FInputModeGameOnly GameOnlyInputMode;
	SetInputMode(GameOnlyInputMode);
}
