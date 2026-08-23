#include "Modules/ModuleManager.h"

#include "Editor.h"
#include "RewindPIEInputToolset.h"
#include "ToolsetRegistry/UToolsetRegistry.h"

class FRewindEditorModule final : public IModuleInterface
{
public:
	virtual void StartupModule() override
	{
		UToolsetRegistry::RegisterToolsetClass(URewindPIEInputToolset::StaticClass());
		EndPIEHandle = FEditorDelegates::EndPIE.AddRaw(this, &FRewindEditorModule::HandleEndPIE);
	}

	virtual void ShutdownModule() override
	{
		if (EndPIEHandle.IsValid())
		{
			FEditorDelegates::EndPIE.Remove(EndPIEHandle);
			EndPIEHandle.Reset();
		}

		URewindPIEInputToolset::ClearTrackedKeys();
		UToolsetRegistry::UnregisterToolsetClass(URewindPIEInputToolset::StaticClass());
	}

private:
	void HandleEndPIE(bool bIsSimulating)
	{
		URewindPIEInputToolset::ClearTrackedKeys();
	}

	FDelegateHandle EndPIEHandle;
};

IMPLEMENT_MODULE(FRewindEditorModule, RewindEditor)
