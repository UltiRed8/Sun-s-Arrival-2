//#include "GIS_Online.h"
//
//#include "SR_Utils.h"
//#include "GameFramework/PlayerState.h"
//#include <Kismet/KismetStringLibrary.h>
//#include <Kismet/GameplayStatics.h>
//#include <GameMapsSettings.h>
//
//#pragma region Init
//void UGIS_Online::Initialize(FSubsystemCollectionBase& _collection)
//{
//	Super::Initialize(_collection);
//
//	printDebugs = true;
//	online = nullptr;
//	sessionFilterGameName = "SunsArrival";
//	isUsingSteam = false;
//	onlineIdentity = nullptr;
//	sessionInterface = nullptr;
//	sessionSearch = nullptr;
//	settings = FOnlineSessionSettings();
//	settings.bAllowInvites = true;
//	settings.bAllowJoinInProgress = true;
//	settings.bAllowJoinViaPresence = true;
//	settings.bAllowJoinViaPresenceFriendsOnly = false;
//	settings.bAntiCheatProtected = false;
//	settings.bIsDedicated = false;
//	settings.bShouldAdvertise = true;
//	settings.bUseLobbiesIfAvailable = true;
//	settings.bUseLobbiesVoiceChatIfAvailable = false;
//	settings.bUsesPresence = true;
//	settings.bUsesStats = false;
//	settings.NumPublicConnections = 3;
//	currentSessionName = "";
//
//	settings.Set(FName("GameSessionsID"), sessionFilterGameName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
//
//	Init();
//}
//
//void UGIS_Online::Init()
//{
//	LOG("Init...");
//
//	online = IOnlineSubsystem::Get();
//	if (!online)
//		return;
//	
//	isUsingSteam = online->GetSubsystemName().ToString() == "STEAM";
//	LOG_WARN("Online subsystem: " + (isUsingSteam ? "Steam" : "Local"));
//	settings.bIsLANMatch = !isUsingSteam;
//	
//	onlineIdentity = online->GetIdentityInterface();
//	if (!onlineIdentity)
//		return;
//	
//	sessionInterface = online->GetSessionInterface();
//	if (!sessionInterface)
//		return;
//	
//	sessionSearch = MakeShareable(new FOnlineSessionSearch());
//	if (!sessionSearch)
//		return;
//	
//	LOG("Init complete!");
//	
//	BindCallbacks();
//}
//
//void UGIS_Online::BindCallbacks()
//{
//	if (!IsOnlineValid())
//		return;
//	LOG("Binding callbacks...");
//	
//	// Main Session Events
//	sessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UGIS_Online::EventCreateSession);
//	sessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UGIS_Online::EventFindSessions);
//	sessionInterface->OnCancelFindSessionsCompleteDelegates.AddUObject(this, &UGIS_Online::EventCancelFindSessions);
//	sessionInterface->OnSessionSettingsUpdatedDelegates.AddUObject(this, &UGIS_Online::EventSessionsSettingsUpdated);
//	sessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &UGIS_Online::EventDestroySession);
//	sessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UGIS_Online::EventJoinedSession);
//
//	// Errors
//	sessionInterface->OnSessionFailureDelegates.AddUObject(this, &UGIS_Online::EventSessionFailure);
//	GEngine->OnNetworkFailure().AddUObject(this, &UGIS_Online::EventNetworkFailure);
//
//	// Gameplay
//	sessionInterface->OnStartSessionCompleteDelegates.AddUObject(this, &UGIS_Online::EventStartSession);
//	sessionInterface->OnEndSessionCompleteDelegates.AddUObject(this, &UGIS_Online::EventEndSession);
//
//	// Invites
//	sessionInterface->OnSessionInviteReceivedDelegates.AddUObject(this, &UGIS_Online::EventSessionInviteReceived);
//	sessionInterface->OnSessionUserInviteAcceptedDelegates.AddUObject(this, &UGIS_Online::EventSessionUserInviteAccepted);
//
//	// Participants
//	sessionInterface->OnSessionParticipantJoinedDelegates.AddUObject(this, &UGIS_Online::EventSessionParticipantJoined);
//	sessionInterface->OnSessionParticipantLeftDelegates.AddUObject(this, &UGIS_Online::EventSessionParticipantLeft);
//		
//	LOG("Callbacks binded!");
//}
//#pragma endregion
//
//#pragma region BlueprintNodes
//void UGIS_Online::CreateSession(const FString& _sessionName, const int _maxSlots, const bool _friendsOnly)
//{
//	if (!IsOnlineValid())
//		return;
//
//	settings.NumPublicConnections = _maxSlots;
//	settings.bAllowJoinViaPresenceFriendsOnly = _friendsOnly;
//	
//	const FString& _randomizedSessionName = _sessionName + FString::SanitizeFloat(FMath::RandRange(0.0, 200.0));
//	settings.Set(FName("SessionName"), _randomizedSessionName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
//	settings.Set(FName("SessionRealName"), _sessionName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
//	settings.Set(FName("CurrentPlayerAmount"), 1, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
//		
//	onCreatingSessions.Broadcast();
//
//	if (!sessionInterface->CreateSession(0, *_randomizedSessionName, settings))
//		GeneralizeError("Error while creating session. Err0");
//}
//
//void UGIS_Online::SearchSessions(const int _maxResults)
//{
//	if (!IsOnlineValid())
//		return;
//		
//	sessionSearch->bIsLanQuery = !isUsingSteam;
//	sessionSearch->MaxSearchResults = _maxResults;
//	sessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);
//		
//	sessionSearch->QuerySettings.Set(FName("GameSessionsID"), sessionFilterGameName, EOnlineComparisonOp::Equals);
//		
//	onSearchingSessions.Broadcast();
//
//	if (!sessionInterface->FindSessions(0, sessionSearch.ToSharedRef()))
//		GeneralizeError("Error while finding sessions. Err1");
//}
//
//void UGIS_Online::CancelSearchSessions()
//{
//	if (!IsOnlineValid())
//		return;
//
//	onSessionSearchCancelling.Broadcast();
//
//	if (!sessionInterface->CancelFindSessions())
//		GeneralizeError("Error while cancelling finding sessions. Err2");
//}
//
//void UGIS_Online::JoinSession(const FSessionData& _session)
//{
//	if (!IsOnlineValid())
//		return;
//		
//	onJoiningSession.Broadcast();
//
//	if (!sessionInterface->JoinSession(0, FName(_session.sessionName), sessionSearch->SearchResults[_session.sessionIndexInList]))
//		GeneralizeError("Error while joining session. Err3");
//}
//
//void UGIS_Online::DestroySession(const FString& _fallbackLevelName)
//{
//	if (!IsOnlineValid())
//		return;
//
//	onDestroyingSessions.Broadcast();
//
//	if (!sessionInterface->DestroySession(FName(currentSessionName)))
//	{
//		UGameplayStatics::OpenLevel(this, FName(_fallbackLevelName));
//		GeneralizeError("Error while leaving session. Err4");
//	}
//}
//
//FString UGIS_Online::GetSteamUsername()
//{
//	if (!IsOnlineValid())
//		return "";
//	const FUniqueNetIdPtr& _userId = onlineIdentity->GetUniquePlayerId(0);
//	return onlineIdentity->GetPlayerNickname(*_userId);
//}
//
//void UGIS_Online::SetCurrentAmountOfPlayers(const int _amount)
//{
//	if (!IsOnlineValid())
//		return;
//
//	settings.Set(FName("CurrentPlayerAmount"), _amount, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
//
//	if (!sessionInterface->UpdateSession(currentSessionName, settings))
//		GeneralizeError("Failed to update session settings. Err16");
//}
//#pragma endregion
//
//#pragma region Events
//void UGIS_Online::EventCreateSession(const FName _sessionName, const bool _success)
//{
//	if (_success)
//	{
//		onSessionCreated.Broadcast();
//		GeneralizeLogMessage("Session created!");
//		currentSessionName = _sessionName;
//	}
//	else
//		GeneralizeError("Session creation error. Err6");
//}
//
//void UGIS_Online::EventFindSessions(const bool _success)
//{
//	if (_success)
//	{
//		const TArray<FOnlineSessionSearchResult>& _results = sessionSearch->SearchResults;
//		TArray<FSessionData> _sessionsData;
//		const int& _size = _results.Num();
//		for (int _index = 0; _index < _size; _index++)
//		{
//			const FOnlineSessionSearchResult& _result = _results[_index];
//			if (!_result.IsValid())
//				continue;
//			_sessionsData.Add(FSessionData(_result, _index));
//		}
//		sessionSearch->SearchResults = _results;
//		onSessionSearchEnded.Broadcast(_sessionsData);
//		GeneralizeLogMessage("Session found!");
//	}
//	else
//		GeneralizeError("Session search error. Err7");
//}
//
//void UGIS_Online::EventCancelFindSessions(const bool _success)
//{
//	if (_success)
//	{
//		onSessionSearchCanceled.Broadcast();
//		GeneralizeLogMessage("Session search cancelled!");
//	}
//	else
//		GeneralizeError("Session search cancelling error. Err8");
//}
//
//void UGIS_Online::EventSessionsSettingsUpdated(const FName _sessionName, const FOnlineSessionSettings& _settings)
//{
//	settings = _settings;
//	GeneralizeLogMessage("Session settings updated!");
//	onSessionSettingsUpdated.Broadcast();
//	currentSessionName = _sessionName;
//}
//
//void UGIS_Online::EventDestroySession(const FName _sessionName, const bool _success)
//{
//	if (_success)
//	{
//		onSessionDestroyed.Broadcast();
//		GeneralizeLogMessage("Session destroyed!");
//		currentSessionName = "";
//	}
//	else
//		GeneralizeError("Session destroy error. Err9");
//}
//
//void UGIS_Online::EventSessionFailure(const FUniqueNetId& _netID, const ESessionFailure::Type _failureType)
//{
//	GeneralizeError("Session failure. UniqueNetID: " + _netID.ToString() + ". Err10");
//}
//
//void UGIS_Online::EventNetworkFailure(UWorld* _world, UNetDriver* _driver, ENetworkFailure::Type _failureType, const FString& _error)
//{
//	GeneralizeError("Network failure type \"" + FString(ENetworkFailure::ToString(_failureType)) + "\". Err11");
//}
//
//void UGIS_Online::EventStartSession(const FName _sessionName, const bool _success)
//{
//	if (_success)
//	{
//		onSessionStart.Broadcast();
//		GeneralizeLogMessage("Session started!");
//	}
//	else
//		GeneralizeError("Session start error. Err12");
//}
//
//void UGIS_Online::EventEndSession(const FName _sessionName, const bool _success)
//{
//	if (_success)
//	{
//		onSessionEnd.Broadcast();
//		GeneralizeLogMessage("Session ended!");
//	}
//	else
//		GeneralizeError("Session end error. Err13");
//}
//
//void UGIS_Online::EventSessionInviteReceived(const FUniqueNetId& _userId, const FUniqueNetId& _fromId, const FString& _appId, const FOnlineSessionSearchResult& _invite)
//{
//	onSessionInviteReceived.Broadcast();
//	GeneralizeLogMessage("Invite received from user id: " + _fromId.ToString());
//}
//
//void UGIS_Online::EventSessionUserInviteAccepted(const bool _success, const int32 _controllerId, FUniqueNetIdPtr _uniqueNetId, const FOnlineSessionSearchResult& _invite)
//{
//	if (_success)
//	{
//		GeneralizeLogMessage("Session invite accepted!");
//		
//		FString _sessionName = "";
//		_invite.Session.SessionSettings.Get(FName("SessionName"), _sessionName);
//
//		onSessionInviteAccepted.Broadcast();
//
//		if (!sessionInterface->JoinSession(0, *_sessionName, _invite))
//			GeneralizeError("Error while joining session. Err14");
//	}
//	else
//		GeneralizeError("Session invite error. Err15");
//}
//
//void UGIS_Online::EventSessionParticipantJoined(const FName _sessionName, const FUniqueNetId& _userId)
//{
//	onParticipantJoined.Broadcast();
//	GeneralizeLogMessage("User " + _userId.ToString() + " joined the session!");
//}
//
//void UGIS_Online::EventSessionParticipantLeft(const FName _sessionName, const FUniqueNetId& _userId, const EOnSessionParticipantLeftReason _leftReason)
//{
//	onParticipantLeft.Broadcast();
//	GeneralizeLogMessage("User " + _userId.ToString() + " left the session!");
//}
//
//void UGIS_Online::EventJoinedSession(const FName _sessionName, const EOnJoinSessionCompleteResult::Type _joinSessionType)
//{
//	currentSessionName = _sessionName;
//	FString _connectString;
//	if (sessionInterface->GetResolvedConnectString(_sessionName, _connectString))
//	{
//		if (CAST(APlayerController, _playerController, GetGameInstance()->GetFirstLocalPlayerController()))
//		{
//			_playerController->ClientTravel(_connectString, ETravelType::TRAVEL_Absolute);
//			onSessionJoined.Broadcast();
//		}
//	}
//	GeneralizeLogMessage("Joined session!");
//}
//#pragma endregion
//
//bool UGIS_Online::IsOnlineValid()
//{
//	const bool& _isValid = (online && onlineIdentity && sessionInterface && sessionSearch);
//	if (!_isValid)
//		GeneralizeError("Online is invalid. Err15");
//	return _isValid;
//}
//
//void UGIS_Online::GeneralizeLogMessage(const FString& _message)
//{
//	if (printDebugs)
//		LOG(_message);
//}
//
//void UGIS_Online::GeneralizeError(const FString& _message)
//{
//	if (printDebugs)
//		LOG_ERROR(_message);
//	onErrorDetected.Broadcast(_message);
//}