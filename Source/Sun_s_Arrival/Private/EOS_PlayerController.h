#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "EOS_PlayerController.generated.h"

UCLASS()
class AEOS_PlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	virtual void OnNetCleanup(UNetConnection* _connection) override;
};
