#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "CustomBlueprintFunctionLibrary.generated.h"

UCLASS()
class UCustomBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category="Utility")
	static bool IsGameWorld(const UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, Category = "Utility")
	static float GetFPS();

	UFUNCTION(BlueprintCallable, Category = "PSO Compilation")
	static int32 GetRemainingShaders();

	UFUNCTION(BlueprintCallable, Category = "PSO Compilation")
	static void CompilePSO();
};
