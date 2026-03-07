// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT
#pragma once

class JUTILS_API FJUtilsModule : public IModuleInterface {
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
