#pragma once
#ifdef _DLL
#define WIS_EXPORT __declspec(dllexport)
#else
#define WIS_EXPORT __declspec(dllimport)
#endif // _DLL

int WIS_EXPORT Xmain();