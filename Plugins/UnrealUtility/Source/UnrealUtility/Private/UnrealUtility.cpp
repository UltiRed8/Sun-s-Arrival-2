#include "UnrealUtility.h"
#include "UnrealUtilityStyle.h"
#include "UnrealUtilityCommands.h"
#include "ToolMenus.h"
#include "Editor.h"
#include "Sound/SoundBase.h"
#include "Kismet/GameplayStatics.h"
#include "AudioDevice.h"
#include "EditorModeManager.h"
#include "LevelEditor.h"
#include "Framework/MultiBox/MultiBoxBuilder.h"
#include "Misc/Paths.h"
#include "HAL/PlatformProcess.h"
#include "Editor/EditorStyle/Public/EditorStyleSet.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "TickableEditorObject.h"
#include "Engine/Blueprint.h"
#include "Misc/FileHelper.h"
#include "HAL/FileManager.h"
#include "DesktopPlatformModule.h"
#include "IDesktopPlatform.h"
#include "Settings/LevelEditorMiscSettings.h"
#include "UnrealUtilitySettings.h"
#include "ISettingsModule.h"
#include "ISettingsSection.h"

static const FName UnrealUtilityTabName("UnrealUtility");

#define LOCTEXT_NAMESPACE "FUnrealUtilityModule"

void FUnrealUtilityModule::StartupModule()
{
	FUnrealUtilityStyle::Initialize();
	FUnrealUtilityStyle::ReloadTextures();

	FEditorDelegates::PostPIEStarted.AddRaw(this, &FUnrealUtilityModule::OnBeginPIE);
	FEditorDelegates::EndPIE.AddRaw(this, &FUnrealUtilityModule::OnEndPIE);

	FUnrealUtilityCommands::Register();

	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(FUnrealUtilityCommands::Get().RestartAction, FExecuteAction::CreateRaw(this, &FUnrealUtilityModule::RestartEditorClicked), FCanExecuteAction());
	PluginCommands->MapAction(FUnrealUtilityCommands::Get().CleanRebuildAction, FExecuteAction::CreateRaw(this, &FUnrealUtilityModule::CleanRebuildRestart), FCanExecuteAction());

	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FUnrealUtilityModule::RegisterMenus));

	if (ISettingsModule* _settingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		_settingsModule->RegisterSettings(
			"Editor",
			"Unreal Utility",
			"Sounds",
			LOCTEXT("UnrealUtilityName", "Sounds"),
			LOCTEXT("UnrealUtilityDesc", "Audio settings for Unreal Utility."),
			GetMutableDefault<UUnrealUtilitySettings>()
		);
	}
}

void FUnrealUtilityModule::ShutdownModule()
{
	if (UObjectInitialized())
	{
		FEditorDelegates::BeginPIE.RemoveAll(this);
		FEditorDelegates::EndPIE.RemoveAll(this);
	}
	if (ISettingsModule* _settingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		_settingsModule->UnregisterSettings("Editor", "Unreal Utility", "Sounds");
	}
}

void FUnrealUtilityModule::RegisterMenus()
{
	FToolMenuOwnerScoped _ownerScoped(this);
	{
		UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Window");
		{
			FToolMenuSection& Section = Menu->FindOrAddSection("WindowLayout");
			Section.AddMenuEntryWithCommandList(FUnrealUtilityCommands::Get().RestartAction, PluginCommands);
			Section.AddMenuEntryWithCommandList(FUnrealUtilityCommands::Get().CleanRebuildAction, PluginCommands);
		}
	}

	{
		UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar.PlayToolBar");
		{
			FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("PluginTools");
			{
				FToolMenuEntry& Entry = Section.AddEntry(FToolMenuEntry::InitToolBarButton(FUnrealUtilityCommands::Get().RestartAction));
				Entry.SetCommandList(PluginCommands);
			}
		}
	}

	{
		UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar.PlayToolBar");
		{
			FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("PluginTools");
			{
				FToolMenuEntry& Entry = Section.AddEntry(FToolMenuEntry::InitToolBarButton(FUnrealUtilityCommands::Get().CleanRebuildAction));
				Entry.SetCommandList(PluginCommands);
			}
		}
	}
}

void FUnrealUtilityModule::RestartEditorClicked()
{
	FUnrealEdMisc::Get().RestartEditor();
}

void FUnrealUtilityModule::CleanRebuildRestart()
{
	FString _projectFile = FPaths::ConvertRelativePathToFull(FPaths::GetProjectFilePath());
	FString _projectDir = FPaths::GetPath(_projectFile);
	FString _engineExe = FPlatformProcess::ExecutablePath();

	FString _scriptPath = _projectDir / TEXT("RebuildAndRestart.bat");

	FString _scriptContent = FString::Printf(TEXT(
		"@echo off\n"
		"setlocal\n"
		"echo ======================================\n"
		"echo  Unreal Project Full Rebuild Script\n"
		"echo ======================================\n"
		"\n"
		"echo Waiting for Unreal Editor to close...\n"
		":wait_for_editor\n"
		"tasklist /FI \"IMAGENAME eq UnrealEditor.exe\" | find /I \"UnrealEditor.exe\" >nul\n"
		"if %%errorlevel%%==0 (\n"
		"    timeout /t 2 >nul\n"
		"    goto wait_for_editor\n"
		")\n"
		"echo Unreal Editor closed.\n"
		"\n"
		"echo.\n"
		"echo Deleting old build folders...\n"
		"rmdir /s /q \"%s\\Binaries\"\n"
		"rmdir /s /q \"%s\\Intermediate\"\n"
		"del /q \"%s\\*.sln\" >nul 2>&1\n"
		"\n"
		"echo.\n"
		"echo Regenerating Visual Studio project files...\n"
		"set UBT_PATH=\"%s\\Engine\\Binaries\\DotNET\\UnrealBuildTool\\UnrealBuildTool.exe\"\n"
		"if not exist %%UBT_PATH%% set UBT_PATH=\"%s\\Engine\\Binaries\\DotNET\\UnrealBuildTool.exe\"\n"
		"if not exist %%UBT_PATH%% (\n"
		"    echo [ERROR] UnrealBuildTool.exe not found!\n"
		"    pause\n"
		"    exit /b 1\n"
		")\n"
		"%%UBT_PATH%% -projectfiles -project=\"%s\" -game -engine -progress\n"
		"if %%errorlevel%% neq 0 (\n"
		"    echo [ERROR] Failed to generate project files.\n"
		"    pause\n"
		"    exit /b %%errorlevel%%\n"
		")\n"
		"\n"
		"echo.\n"
		"echo Compiling project...\n"
		"%%UBT_PATH%% %sEditor Win64 Development -project=\"%s\" -progress\n"
		"if %%errorlevel%% neq 0 (\n"
		"    echo [ERROR] Build failed.\n"
		"    pause\n"
		"    exit /b %%errorlevel%%\n"
		")\n"
		"\n"
		"echo.\n"
		"echo Launching Unreal Editor...\n"
		"start \"\" \"%s\" \"%s\" -Build\n"
		"\n"
		"echo.\n"
		"echo All tasks completed successfully!\n"
		"pause\n"
		"endlocal\n"
	),
		*_projectDir,
		*_projectDir,
		*_projectDir,
		*FPaths::RootDir(),
		*FPaths::RootDir(),
		*_projectFile,
		*FPaths::GetBaseFilename(_projectFile),
		*_projectFile,
		*_engineExe,
		*_projectFile
	);

	FFileHelper::SaveStringToFile(_scriptContent, *_scriptPath);

	FPlatformProcess::CreateProc(*_scriptPath, nullptr, false, false, false, nullptr, 0, *_projectDir, nullptr);

	FGenericPlatformMisc::RequestExit(false);
}

void FUnrealUtilityModule::OnBeginPIE(bool bIsSimulating)
{
	UE_LOG(LogTemp, Log, TEXT("Started"));
	FTimerHandle TimerHandle;
	GEditor->GetTimerManager()->SetTimer(TimerHandle, [this]()
	{
		PlayEditorSound(TEXT("/Script/Engine.SoundWave'/Engine/EditorSounds/GamePreview/StartPlayInEditor.StartPlayInEditor'"), false);
	}, 0.1f, false);
}

void FUnrealUtilityModule::OnEndPIE(bool bIsSimulating)
{
	PlayEditorSound(TEXT("/Script/Engine.SoundWave'/Engine/EditorSounds/GamePreview/EndPlayInEditor.EndPlayInEditor'"), true);
}

void FUnrealUtilityModule::PlayEditorSound(const FString& Path, const bool& _isEditorSound)
{
	const UUnrealUtilitySettings* _settings = GetDefault<UUnrealUtilitySettings>();
	if (!_settings->GetRestoreEditorSounds())
		return;

	USoundBase* _sound = LoadObject<USoundBase>(nullptr, *Path);
	if (!_sound)
	{
		UE_LOG(LogTemp, Warning, TEXT("[EditorSoundsRestored] Failed to load sound: %s"), *Path);
		return;
	}

	if (_isEditorSound)
		GEditor->PlayEditorSound(_sound);
	else
		UGameplayStatics::PlaySound2D(GEditor->PlayWorld, _sound);
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FUnrealUtilityModule, UnrealUtility)