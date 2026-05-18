#include "EOS_GameMode.h"
//#include "OnlineSubsystemUtils.h"
//#include "OnlineSubsystem.h"
//#include "Interfaces/OnlineIdentityInterface.h"
//#include "Interfaces/OnlineSessionInterface.h"

void AEOS_GameMode::PostLogin(APlayerController* _newPlayer)
{
	Super::PostLogin(_newPlayer);
	/*if (_newPlayer)
	{
		FUniqueNetIdRepl _uniqueNetIDRepl;
		if (_newPlayer->IsLocalController())
		{
			ULocalPlayer* _localPlayerRef = _newPlayer->GetLocalPlayer();
			if (_localPlayerRef)
			{
				_uniqueNetIDRepl = _localPlayerRef->GetPreferredUniqueNetId();
			}
			else
			{
				UNetConnection* _removeNetConnectionRef = Cast<UNetConnection>(_newPlayer->Player);
				check(IsValid(_removeNetConnectionRef));
				_uniqueNetIDRepl = _removeNetConnectionRef->PlayerId;
			}
		}
		else
		{
			UNetConnection* _removeNetConnectionRef = Cast<UNetConnection>(_newPlayer->Player);
			check(IsValid(_removeNetConnectionRef));
			_uniqueNetIDRepl = _removeNetConnectionRef->PlayerId;
		}

		TSharedPtr<const FUniqueNetId> _uniqueNetId = _uniqueNetIDRepl.GetUniqueNetId();
		if (_uniqueNetId != nullptr)
		{
			IOnlineSubsystem* _subsystem = Online::GetSubsystem(_newPlayer->GetWorld());
			IOnlineSessionPtr _sessionRef = _subsystem->GetSessionInterface();
			bool _registrationSuccess = _sessionRef->RegisterPlayer(FName("MainSession_GameSession"), *_uniqueNetId, false);
			if (_registrationSuccess)
			{
				UE_LOG(LogTemp, Warning, TEXT("Registration successful"));
			}
		}
	}*/
}
