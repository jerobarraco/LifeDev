#pragma once


class UInputMappingContext;
class UWorld;
// TODO turn this into a blueprintlibrary

JUTILS_API extern UWorld* JGetWorld(UWorld* World);
JUTILS_API extern void ToggleMapping(
	UInputMappingContext* Ctx, int32 Prio = 0, bool Enable = false, UWorld* World = nullptr) ;
