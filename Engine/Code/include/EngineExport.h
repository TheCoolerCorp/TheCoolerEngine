#ifndef ENGINEEXPORT_H
#define ENGINEEXPORT_H

#ifdef ENGINE_EXPORTS
	#define ENGINE_API __declspec(dllexport)
#else
	#define ENGINE_API __declspec(dllimport)
#endif

#endif