#ifndef ENGINEEXPORT_H
#define ENGINEEXPORT_H

#ifdef Engine_EXPORTS
	#ifdef _MSC_VER
		#define ENGINE_API __declspec(dllexport)
	#else
		#define ENGINE_API __attribute__((visibility("default")))
	#endif
#else
	#ifdef _MSC_VER
		#define ENGINE_API __declspec(dllimport)
	#else
		#define ENGINE_API
	#endif
#endif

#if defined(_DEBUG) || defined(DEBUG) || defined(__DEBUG__) || defined(__DEBUG)
	#define MY_DEBUG
#endif

#endif