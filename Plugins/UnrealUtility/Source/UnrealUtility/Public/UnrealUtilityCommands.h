#pragma once

#include "Framework/Commands/Commands.h"
#include "UnrealUtilityStyle.h"

class FUnrealUtilityCommands : public TCommands<FUnrealUtilityCommands>
{
public:
	TSharedPtr<FUICommandInfo> RestartAction;
	TSharedPtr<FUICommandInfo> CleanRebuildAction;

public:
	FUnrealUtilityCommands() : TCommands<FUnrealUtilityCommands>(TEXT("UnrealUtility"), NSLOCTEXT("Contexts", "UnrealUtility", "UnrealUtility Plugin"), NAME_None, FUnrealUtilityStyle::GetStyleSetName())
	{
	}
	virtual void RegisterCommands() override;
};
