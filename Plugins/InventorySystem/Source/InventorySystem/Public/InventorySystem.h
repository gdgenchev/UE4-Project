// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FInventorySystemModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
#define CLASS_FUNCTION (FString(__FUNCTION__))

DECLARE_LOG_CATEGORY_EXTERN(LogInventorySystem, Log, All);


