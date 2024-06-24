// Fill out your copyright notice in the Description page of Project Settings.


#include "MyMatchEndWidget.h"
#include "Components/TextBlock.h"
#include "GameFramework/PlayerState.h"
#include "Project2/Game/MyGameMode.h"
#include "Project2/Player/MyPlayerController.h"
#include "EngineUtils.h"

UMyMatchEndWidget::UMyMatchEndWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

void UMyMatchEndWidget::NativeConstruct()
{
	Super::NativeConstruct();

	PlayerScore1 = Cast<UTextBlock>(GetWidgetFromName(TEXT("TxtScore1")));
	ensure(PlayerScore1);

	PlayerScore2 = Cast<UTextBlock>(GetWidgetFromName(TEXT("TxtScore2")));
	ensure(PlayerScore2);


	AMyGameMode* GameMode = Cast<AMyGameMode>(GetWorld()->GetAuthGameMode());
	if (GameMode)
	{
		GameMode->OnEndMatchDelegate.AddUObject(this, &UMyMatchEndWidget::ViewScores);
	}
}

void UMyMatchEndWidget::ViewScores()
{
	// View end screen and score on server side
	AMyPlayerController* Controller = Cast<AMyPlayerController>(GetOwningPlayer());
	if (Controller)
	{
		Controller->MyMatchEndWidget->SetVisibility(ESlateVisibility::Visible);

		APlayerState* PlayerState = Controller->PlayerState;
		if (PlayerState)
		{
			EndScore1 = PlayerState->GetScore();
			PlayerScore1->SetText(FText::FromString(GetScoreText(EndScore1)));
		}
	}

	// View end screen and score on client side
	/*
	int PlayerControllerCount = 0;
	for (APlayerController* PlayerController : TActorRange<APlayerController>(GetWorld()))
	{
		APlayerState* PlayerState = PlayerController->PlayerState;
		if (PlayerState)
		{
			
		}
	}
	*/
}

FString UMyMatchEndWidget::GetScoreText(float PlayerNumber)
{
	return FString::Printf(TEXT("%d"), PlayerNumber);
}