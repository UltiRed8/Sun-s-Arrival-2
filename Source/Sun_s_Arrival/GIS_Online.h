//#pragma once
//
//#include "CoreMinimal.h"
//#include "OnlineSessionSettings.h"
//#include <Kismet/KismetSystemLibrary.h>
//#include "Subsystems/GameInstanceSubsystem.h"
//#include "OnlineSubsystem.h"
//#include "Online/OnlineSessionNames.h"
//#include "Interfaces/OnlineSessionInterface.h"
//#include "Interfaces/OnlineIdentityInterface.h"
//#include "Interfaces/OnlinePresenceInterface.h"
//#include "Interfaces/OnlineFriendsInterface.h"
//#include "Templates/SharedPointer.h"
//#include "GIS_Online.generated.h"
//
//
//USTRUCT(BlueprintType)
//struct FSessionData
//{
//	GENERATED_BODY()
//
//public:
//	UPROPERTY(VisibleAnywhere, BlueprintReadOnly) FString sessionName;
//	UPROPERTY(VisibleAnywhere, BlueprintReadOnly) FString sessionRealName;
//	UPROPERTY(VisibleAnywhere, BlueprintReadOnly) int sessionIndexInList;
//	UPROPERTY(VisibleAnywhere, BlueprintReadOnly) int32 pingMS;
//	UPROPERTY(VisibleAnywhere, BlueprintReadOnly) int32 privateConnections;
//	UPROPERTY(VisibleAnywhere, BlueprintReadOnly) int32 publicConnections;
//	UPROPERTY(VisibleAnywhere, BlueprintReadOnly) int32 currentPlayers;
//	UPROPERTY(VisibleAnywhere, BlueprintReadOnly) FString owningUserName;
//
//public:
//	FSessionData() = default;
//	FSessionData(const FOnlineSessionSearchResult& _session, const int& _index)
//	{
//		_session.Session.SessionSettings.Get(FName("SessionName"), sessionName);
//		_session.Session.SessionSettings.Get(FName("SessionRealName"), sessionRealName);
//		_session.Session.SessionSettings.Get(FName("CurrentPlayerAmount"), currentPlayers);
//		pingMS = _session.PingInMs;
//		privateConnections = _session.Session.NumOpenPrivateConnections;
//		publicConnections = _session.Session.NumOpenPublicConnections;
//		owningUserName = _session.Session.OwningUserName;
//		sessionIndexInList = _index;
//		currentPlayers = 1;
//	}
//};
//
//UCLASS(BlueprintType)
//class SUN_S_ARRIVAL_API UGIS_Online : public UGameInstanceSubsystem
//{
//	GENERATED_BODY()
//
//private:
//	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCustomOnErrorDetected, const FString&, _message);
//	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FCustomOnCreatingSessions);
//	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FCustomOnSessionCreated);
//	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FCustomOnSearchingSessions);
//	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCustomOnSessionSearchEnded, const TArray<FSessionData>&, _sessions);
//	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FCustomOnSessionSearchCancelling);
//	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FCustomOnSessionSearchCanceled);
//	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FCustomOnJoiningSession);
//	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FCustomOnSessionJoined);
//	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FCustomOnDestroyingSessions);
//	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FCustomOnSessionDestroyed);
//	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FCustomOnSessionSettingsUpdated);
//	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FCustomOnSessionStart);
//	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FCustomOnSessionEnd);
//	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FCustomOnSessionInviteReceived);
//	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FCustomOnSessionInviteAccepted);
//	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FCustomOnParticipantJoined);
//	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FCustomOnParticipantLeft);
//
//protected:
//	UPROPERTY(BlueprintReadOnly, BlueprintAssignable) FCustomOnErrorDetected onErrorDetected;
//	UPROPERTY(BlueprintReadOnly, BlueprintAssignable) FCustomOnCreatingSessions onCreatingSessions;
//	UPROPERTY(BlueprintReadOnly, BlueprintAssignable) FCustomOnSessionCreated onSessionCreated;
//	UPROPERTY(BlueprintReadOnly, BlueprintAssignable) FCustomOnSearchingSessions onSearchingSessions;
//	UPROPERTY(BlueprintReadOnly, BlueprintAssignable) FCustomOnSessionSearchEnded onSessionSearchEnded;
//	UPROPERTY(BlueprintReadOnly, BlueprintAssignable) FCustomOnSessionSearchCancelling onSessionSearchCancelling;
//	UPROPERTY(BlueprintReadOnly, BlueprintAssignable) FCustomOnSessionSearchCanceled onSessionSearchCanceled;
//	UPROPERTY(BlueprintReadOnly, BlueprintAssignable) FCustomOnJoiningSession onJoiningSession;
//	UPROPERTY(BlueprintReadOnly, BlueprintAssignable) FCustomOnSessionJoined onSessionJoined;
//	UPROPERTY(BlueprintReadOnly, BlueprintAssignable) FCustomOnDestroyingSessions onDestroyingSessions;
//	UPROPERTY(BlueprintReadOnly, BlueprintAssignable) FCustomOnSessionDestroyed onSessionDestroyed;
//	UPROPERTY(BlueprintReadOnly, BlueprintAssignable) FCustomOnSessionSettingsUpdated onSessionSettingsUpdated;
//	UPROPERTY(BlueprintReadOnly, BlueprintAssignable) FCustomOnSessionStart onSessionStart;
//	UPROPERTY(BlueprintReadOnly, BlueprintAssignable) FCustomOnSessionEnd onSessionEnd;
//	UPROPERTY(BlueprintReadOnly, BlueprintAssignable) FCustomOnSessionInviteReceived onSessionInviteReceived;
//	UPROPERTY(BlueprintReadOnly, BlueprintAssignable) FCustomOnSessionInviteAccepted onSessionInviteAccepted;
//	UPROPERTY(BlueprintReadOnly, BlueprintAssignable) FCustomOnParticipantJoined onParticipantJoined;
//	UPROPERTY(BlueprintReadOnly, BlueprintAssignable) FCustomOnParticipantLeft onParticipantLeft;
//
//private:
//	FOnlineSessionSettings settings;
//	bool printDebugs;
//	IOnlineSubsystem* online;
//	IOnlineIdentityPtr onlineIdentity;
//	IOnlineSessionPtr sessionInterface;
//	TSharedPtr<FOnlineSessionSearch> sessionSearch;
//	FString sessionFilterGameName;
//	bool isUsingSteam;
//	FName currentSessionName;
//
//public:
//	UFUNCTION(BlueprintCallable) FORCEINLINE bool IsUsingSteam() const { return isUsingSteam; }
//
//private:
//	virtual void Initialize(FSubsystemCollectionBase& _collection) override;
//	void Init();
//	void BindCallbacks();
//
//public:
//	UFUNCTION(BlueprintCallable) void CreateSession(const FString& _sessionName, const int _maxSlots, const bool _friendsOnly);
//	UFUNCTION(BlueprintCallable) void SearchSessions(const int _maxResults = 50);
//	UFUNCTION(BlueprintCallable) void CancelSearchSessions();
//	UFUNCTION(BlueprintCallable) void JoinSession(const FSessionData& _session);
//	UFUNCTION(BlueprintCallable) void DestroySession(const FString& _fallbackLevelName);
//	UFUNCTION(BlueprintCallable) FString GetSteamUsername();
//	UFUNCTION(BlueprintCallable) void SetCurrentAmountOfPlayers(const int _amount);
//
//private:
//	void EventCreateSession(const FName _sessionName, const bool _success);
//	void EventFindSessions(const bool _success);
//	void EventCancelFindSessions(const bool _success);
//	void EventSessionsSettingsUpdated(const FName _sessionName, const FOnlineSessionSettings& _settings);
//	void EventDestroySession(const FName _sessionName, const bool _success);
//	void EventSessionFailure(const FUniqueNetId& _netID, const ESessionFailure::Type _failureType);
//	void EventNetworkFailure(UWorld* _world, UNetDriver* _driver, const ENetworkFailure::Type _failureType, const FString& _error);
//	void EventStartSession(const FName _sessionName, const bool _success);
//	void EventEndSession(const FName _sessionName, const bool _success);
//	void EventSessionInviteReceived(const FUniqueNetId& _userId, const FUniqueNetId& _fromId, const FString& _appId, const FOnlineSessionSearchResult& _invite);
//	void EventSessionUserInviteAccepted(const bool _success, const int32 _controllerId, FUniqueNetIdPtr _uniqueNetId, const FOnlineSessionSearchResult& _invite);
//	void EventSessionParticipantJoined(const FName _sessionName, const FUniqueNetId& _userId);
//	void EventSessionParticipantLeft(const FName _sessionName, const FUniqueNetId& _userId, const EOnSessionParticipantLeftReason _leftReason);
//	void EventJoinedSession(const FName _sessionName, const EOnJoinSessionCompleteResult::Type _joinSessionType);
//
//private:
//	bool IsOnlineValid();
//	void GeneralizeLogMessage(const FString& _message);
//	void GeneralizeError(const FString& _message);
//};