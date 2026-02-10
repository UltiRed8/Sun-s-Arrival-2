#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "EOS_GameInstance.generated.h"

UCLASS()
class UEOS_GameInstance : public UGameInstance
{
	GENERATED_BODY()
	
private:
	// Event declarations
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLoginStatusChanged, const bool, _isConnected);

protected:
	// Event variables
	UPROPERTY(EditAnywhere, BlueprintAssignable) FOnLoginStatusChanged onLoginStatusChanged;

protected:
	// Variables
	TSharedPtr<FOnlineSessionSearch> sessionSearch;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EOS Variables") FString openLevelText;

public:
	// Public Methods
	UFUNCTION(BlueprintCallable, Category = "EOS Functions") void LoginWithEOS(const FString& _id, const FString& _token, const FString& _loginType);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "EOS Functions") FString GetPlayerUsername();
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "EOS Functions") bool GetPlayerLoginStatus();
	UFUNCTION(BLueprintCallable, Category = "EOS Functions") void CreateEOSSession(const bool _isDedicatedServer, const bool _isLanServer, const int32 _numberOfPublicConnections);
	UFUNCTION(BlueprintCallable, Category = "EOS Functions") void FindSessionAndJoin();
	UFUNCTION(BlueprintCallable, Category = "EOS Functions") void JoinSession();
	UFUNCTION(BlueprintCallable, Category = "EOS Functions") void DestroySession();

private:
	// Methods callbacks
	void LoginWithEOS_Return(int32 _localUserNum, bool _wasSuccess, const FUniqueNetId& _userId, const FString& _error);
	void OnCreateSessionCompleted(FName _sessionName, bool _wasSuccessful);
	void OnDestroySessionCompleted(FName _sessionName, bool _wasSuccessful);
	void OnFindSessionCompleted(bool _wasSuccessful);
	void OnJoinSessionCompleted(FName _sessionName, EOnJoinSessionCompleteResult::Type _result);
};
