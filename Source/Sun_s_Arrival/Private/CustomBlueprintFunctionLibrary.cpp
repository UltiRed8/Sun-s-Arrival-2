#include "CustomBlueprintFunctionLibrary.h"

bool UCustomBlueprintFunctionLibrary::IsGameWorld(const UObject* WorldContextObject)
{
	if (!WorldContextObject)
		return false;
	TObjectPtr<UWorld> _world = WorldContextObject->GetWorld();
	if (!_world)
		return false;
	return _world->IsGameWorld();
}
