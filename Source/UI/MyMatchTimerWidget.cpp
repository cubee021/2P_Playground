// Fill out your copyright notice in the Description page of Project Settings.


#include "MyMatchTimerWidget.h"
#include "Project2/Interface/MyCharacterHUDInterface.h"
#include "Project2/Game/MyGameState.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"

UMyMatchTimerWidget::UMyMatchTimerWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

void UMyMatchTimerWidget::NativeConstruct()
{
	Super::NativeConstruct();

	Timer = Cast<UTextBlock>(GetWidgetFromName(TEXT("TxtTimer")));
	ensure(Timer);

	/*
	UMyCharacterHUDInterface* HUDPawn = Cast<UMyCharacterHUDInterface>(GetOwningPlayerPawn());
	if (HUDPawn)
	{
		//HUDPawn이 nullptr가 반환돼서 사용할 수 없음
	}
	*/
}

/**
* 인터페이스로 CurrentTime을 받아올 수 없어 NativeTick으로 받아오게 함
*/
void UMyMatchTimerWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	AMyGameState* GameState = Cast<AMyGameState>(UGameplayStatics::GetGameState(GetWorld()));
	if (GameState)
	{
		UpdateTimer(GameState->RemainingTime);
	}
}

void UMyMatchTimerWidget::UpdateTimer(float NewTime)
{
	CurrentTime = NewTime;

	ensure(CurrentTime >= 0);

	if (Timer)
	{
		Timer->SetText(FText::FromString(GetTimerText()));
	}
}

FString UMyMatchTimerWidget::GetTimerText()
{
	return FString::Printf(TEXT("%.0f"), CurrentTime);
}
