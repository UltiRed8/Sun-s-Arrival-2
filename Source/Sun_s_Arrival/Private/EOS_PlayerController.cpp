#include "EOS_PlayerController.h"
#include <EOS_GameInstance.h>

void AEOS_PlayerController::OnNetCleanup(UNetConnection* _connection)
{
	UEOS_GameInstance* _gameInstance = Cast<UEOS_GameInstance>(GetWorld()->GetGameInstance());
	if (_gameInstance)
		_gameInstance->DestroySession();
	Super::OnNetCleanup(_connection);
}
