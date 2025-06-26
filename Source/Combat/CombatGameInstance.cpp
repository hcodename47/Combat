#include "CombatGameInstance.h"
#include "Engine/GameInstance.h"
#include "MoviePlayer.h"

void UCombatGameInstance::Init()
{
	Super::Init();

	FCoreUObjectDelegates::PreLoadMap.AddUObject(this, &UCombatGameInstance::BeginLoadingScreen);
	FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, &UCombatGameInstance::EndLoadingScreen);
}

void UCombatGameInstance::BeginLoadingScreen(const FString& InMapName)
{
	if (!IsRunningDedicatedServer())
	{
		FLoadingScreenAttributes LoadingScreen;
		LoadingScreen.bAutoCompleteWhenLoadingCompletes = false;
		LoadingScreen.WidgetLoadingScreen = FLoadingScreenAttributes::NewTestLoadingScreenWidget();
		//LoadingScreen.WidgetLoadingScreen = CreateWidget<UUserWidget>()

		//LoadingScreen.bAutoCompleteWhenLoadingCompletes = true;
		//LoadingScreen.MinimumLoadingScreenDisplayTime = 6000;
		//LoadingScreen.bMoviesAreSkippable = false;
		//LoadingScreen.bWaitForManualStop = false;
		LoadingScreen.bAllowEngineTick = true;

		GetMoviePlayer()->SetupLoadingScreen(LoadingScreen);
		GetMoviePlayer()->PlayMovie();
	}
}

void UCombatGameInstance::EndLoadingScreen(UWorld* InLoadedWorld)
{
	GetMoviePlayer()->StopMovie();
}