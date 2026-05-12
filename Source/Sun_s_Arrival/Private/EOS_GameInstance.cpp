#include "EOS_GameInstance.h"
#include "Sun_s_Arrival.h"
#include <Kismet/GameplayStatics.h>

void UEOS_GameInstance::LoginWithEOS(const FString& _type, const bool _openPortalOnFail)
{
	targetLoginType = _type;
	openPortalOnFail = _openPortalOnFail;
	ConnectWithEOS();
}

void UEOS_GameInstance::DisconnectFromEOS()
{
	IOnlineSubsystem* _subsystem = Online::GetSubsystem(GetWorld());
	if (!_subsystem)
	{
		UE_LOG(LogTemp, Error, TEXT("No OnlineSubsystem"));
		return;
	}
	IOnlineIdentityPtr _identity = _subsystem->GetIdentityInterface();
	if (!_identity)
	{
		UE_LOG(LogTemp, Error, TEXT("No IdentityInterface"));
		return;
	}
	_identity->Logout(0);
	localNetId.Reset();
	UE_LOG(LogTemp, Warning, TEXT("EOS Account Disconnected"));
	onLoginStatusChanged.Broadcast(false);
}

FString UEOS_GameInstance::GetPlayerUsername()
{
	IOnlineSubsystem* _subsystem = Online::GetSubsystem(this->GetWorld());
	if (!_subsystem) return "";
	IOnlineIdentityPtr _identity = _subsystem->GetIdentityInterface();
	if (!_identity) return "";
	if (_identity->GetLoginStatus(0) != ELoginStatus::LoggedIn) return "";
	return _identity->GetPlayerNickname(0);
}

bool UEOS_GameInstance::GetPlayerLoginStatus()
{
	IOnlineSubsystem* _subsystem = Online::GetSubsystem(this->GetWorld());
	if (!_subsystem) return false;
	IOnlineIdentityPtr _identity = _subsystem->GetIdentityInterface();
	if (!_identity) return false;
	return _identity->GetLoginStatus(0) == ELoginStatus::LoggedIn;
}

//void UEOS_GameInstance::CreateEOSSession(const bool _isDedicatedServer, const bool _isLanServer, const int32 _numberOfPublicConnections)
//{
//	IOnlineSubsystem* _subsystem = Online::GetSubsystem(this->GetWorld());
//	if (!_subsystem) return;
//	IOnlineSessionPtr _sessionPtrReference = _subsystem->GetSessionInterface();
//	if (!_sessionPtrReference) return;
//	FUniqueNetIdRepl _netId = GetNetID();
//	if (!_netId.IsValid()) return;
//	FOnlineSessionSettings _settings;
//	_settings.bIsDedicated = _isDedicatedServer;
//	_settings.bAllowInvites = true;
//	_settings.bIsLANMatch = _isLanServer;
//	_settings.NumPublicConnections = _numberOfPublicConnections;
//	_settings.bUseLobbiesIfAvailable = true;
//	_settings.bUsesPresence = true;
//	_settings.bAllowJoinViaPresence = true;
//	_settings.bAllowJoinInProgress = true;
//	_settings.bAllowJoinViaPresenceFriendsOnly = false;
//	_settings.bShouldAdvertise = true;
//	_settings.Set("SESSION_ID", FString("Oui"), EOnlineDataAdvertisementType::ViaOnlineService);
//	_sessionPtrReference->OnCreateSessionCompleteDelegates.AddUObject(this, &UEOS_GameInstance::OnCreateSessionCompleted);
//	_sessionPtrReference->CreateSession(*_netId, FName("MainSession_GameSession"), _settings);
//}

//void UEOS_GameInstance::FindSessionAndJoin()
//{
//	IOnlineSubsystem* _subsystem = Online::GetSubsystem(this->GetWorld());
//	if (!_subsystem) return;
//	IOnlineSessionPtr _sessionPtrReference = _subsystem->GetSessionInterface();
//	if (!_sessionPtrReference) return;
//	FUniqueNetIdRepl _netId = GetNetID();
//	if (!_netId.IsValid()) return;
//	sessionSearch = MakeShareable(new FOnlineSessionSearch());
//	sessionSearch->bIsLanQuery = false;
//	sessionSearch->MaxSearchResults = 20;
//	//sessionSearch->QuerySettings.SearchParams.Empty();
//	sessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);
//	_sessionPtrReference->OnFindSessionsCompleteDelegates.AddUObject(this, &UEOS_GameInstance::OnFindSessionCompleted);
//	_sessionPtrReference->FindSessions(*_netId, sessionSearch.ToSharedRef());
//}

//void UEOS_GameInstance::JoinSession()
//{
//}

//void UEOS_GameInstance::DestroySession()
//{
//	IOnlineSubsystem* _subsystem = Online::GetSubsystem(this->GetWorld());
//	if (!_subsystem) return;
//	IOnlineSessionPtr _sessionPtrReference = _subsystem->GetSessionInterface();
//	if (!_sessionPtrReference) return;
//	_sessionPtrReference->OnDestroySessionCompleteDelegates.AddUObject(this, &UEOS_GameInstance::OnDestroySessionCompleted);
//	_sessionPtrReference->DestroySession(FName("MainSession_GameSession"));
//}

//FUniqueNetIdRepl UEOS_GameInstance::GetNetID()
//{
//	ULocalPlayer* _localPlayer = GetWorld()->GetFirstLocalPlayerFromController();
//	if (!_localPlayer) return FUniqueNetIdRepl();
//	FUniqueNetIdRepl _uniqueNetIdRepl = _localPlayer->GetPreferredUniqueNetId();
//	if (!_uniqueNetIdRepl.IsValid()) return FUniqueNetIdRepl();
//	return _uniqueNetIdRepl;
//}

void UEOS_GameInstance::GiveAchievement(const FString& _achievementID)
{
	if (!GetPlayerLoginStatus())
		return;
	if (!achievementsInterface.IsValid() || !localNetId.IsValid())
		return;
	FOnlineAchievementsWriteRef _writeObject = MakeShared<FOnlineAchievementsWrite>();
	_writeObject->SetFloatStat(FName(*_achievementID), 1.0f);
	achievementsInterface->WriteAchievements( *localNetId, _writeObject,
		FOnAchievementsWrittenDelegate::CreateLambda([](const FUniqueNetId& UserId, bool bSuccess)
			{
				UE_LOG(LogTemp, Warning, TEXT("Achievement result: %s"), bSuccess ? TEXT("SUCCESS") : TEXT("FAILED"));
			}
		)
	);
}

void UEOS_GameInstance::ConnectWithEOS()
{
	IOnlineSubsystem* _subsystem = Online::GetSubsystem(this->GetWorld());
	if (!_subsystem) return;
	IOnlineIdentityPtr _identity = _subsystem->GetIdentityInterface();
	if (!_identity) return;
	FOnlineAccountCredentials _accoundDetails;
	_accoundDetails.Type = targetLoginType;
	_accoundDetails.Id = "";
	_accoundDetails.Token = "";
	_identity->OnLoginCompleteDelegates->AddUObject(this, &UEOS_GameInstance::LoginWithEOS_Return);
	_identity->Login(0, _accoundDetails);
}

void UEOS_GameInstance::LoginWithEOS_Return(int32 _localUserNum, bool _wasSuccess, const FUniqueNetId& _userId, const FString& _error)
{
	onLoginStatusChanged.Broadcast(_wasSuccess);
	if (_wasSuccess)
	{
		UE_LOG(LogTemp, Warning, TEXT("Login Success"));

		localNetId = _userId.AsShared();
		IOnlineSubsystem* _subsystem = Online::GetSubsystem(this->GetWorld());
		achievementsInterface = _subsystem->GetAchievementsInterface();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Login Fail Reason - %s"), *_error);
		if (targetLoginType == "persistentauth" && openPortalOnFail)
			LoginWithEOS("accountportal");
	}
}

//void UEOS_GameInstance::OnCreateSessionCompleted(FName _sessionName, bool _wasSuccessful)
//{
//	if (!_wasSuccessful) return;
//	GetWorld()->ServerTravel(openLevelText);
//}

//void UEOS_GameInstance::OnDestroySessionCompleted(FName _sessionName, bool _wasSuccessful)
//{
//}

//void UEOS_GameInstance::OnFindSessionCompleted(bool _wasSuccessful)
//{
//	if (_wasSuccessful)
//	{
//		IOnlineSubsystem* _subsystem = Online::GetSubsystem(this->GetWorld());
//		if (!_subsystem) return;
//		IOnlineSessionPtr _sessionPtrReference = _subsystem->GetSessionInterface();
//		if (!_sessionPtrReference) return;
//		FUniqueNetIdRepl _netId = GetNetID();
//		if (!_netId.IsValid()) return;
//		if (sessionSearch->SearchResults.Num() > 0)
//		{
//			_sessionPtrReference->OnJoinSessionCompleteDelegates.AddUObject(this, &UEOS_GameInstance::OnJoinSessionCompleted);
//			_sessionPtrReference->JoinSession(*_netId, FName("MainSession_GameSession"), sessionSearch->SearchResults[0]);
//		}
//		else
//		{
//			CreateEOSSession(false, false, 10);
//		}
//	}
//	else
//	{
//		CreateEOSSession(false, false, 10);
//	}
//}

//void UEOS_GameInstance::OnJoinSessionCompleted(FName _sessionName, EOnJoinSessionCompleteResult::Type _result)
//{
//	if (_result == EOnJoinSessionCompleteResult::Success)
//	{
//		if (APlayerController* _playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0))
//		{
//			IOnlineSubsystem* _subsystem = Online::GetSubsystem(this->GetWorld());
//			if (!_subsystem) return;
//			IOnlineSessionPtr _sessionPtrReference = _subsystem->GetSessionInterface();
//			if (!_sessionPtrReference) return;
//			FString _joinAddress;
//			_sessionPtrReference->GetResolvedConnectString(FName("MainSession_GameSession"), _joinAddress);
//			UE_LOG(LogTemp, Warning, TEXT("Join Address is %s"), *_joinAddress);
//			if (_joinAddress.IsEmpty()) return;
//			_playerController->ClientTravel(_joinAddress, ETravelType::TRAVEL_Absolute);
//		}
//	}
//}
