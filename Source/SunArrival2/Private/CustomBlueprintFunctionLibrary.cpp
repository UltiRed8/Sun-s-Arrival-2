#include "CustomBlueprintFunctionLibrary.h"
#include "Engine/Engine.h"
#include <ShaderPipelineCache.h>

extern ENGINE_API float GAverageFPS;

bool UCustomBlueprintFunctionLibrary::IsGameWorld(const UObject* WorldContextObject)
{
	if (!WorldContextObject)
		return false;
	TObjectPtr<UWorld> _world = WorldContextObject->GetWorld();
	if (!_world)
		return false;
	return _world->IsGameWorld();
}

float UCustomBlueprintFunctionLibrary::GetFPS()
{
	return GAverageFPS;
}

int32 UCustomBlueprintFunctionLibrary::GetRemainingShaders()
{
	return FShaderPipelineCache::NumPrecompilesRemaining();
}

void UCustomBlueprintFunctionLibrary::CompilePSO()
{
	FShaderPipelineCache::OpenPipelineFileCache(GMaxRHIShaderPlatform);
	FShaderPipelineCache::SetBatchMode(FShaderPipelineCache::BatchMode::Fast);
}
