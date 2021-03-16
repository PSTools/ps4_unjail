#pragma once

#include "PrxPluginInterface.h"

// THIS IS DEPRECATED.
//
// The new version of this structure (PrxPluginArgsV2) can be found in PrxPluginInterface.h
//
// Please don't change this or use it in new code, instead use the runtime interfaces defined
// in the above IPlugin... header files.
//
// Application data passed from the player.
//
//	THIS MUST MATCH DEFINITIONS IN THE PLAYERS, ALL PLAYERS.
struct PrxPluginArgs
{
	static bool Validate(PrxPluginArgs* arg)
	{
		if ((arg->m_Size != sizeof(PrxPluginArgs)) || (arg->m_Version != s_Version) )
		{
			return false;
		}

		return true;
	}

	PrxPluginArgs() : m_Size(0) ,m_Version(0) {}
	static const uint32_t s_Version = 0x0103;
	uint32_t m_Size;
	uint32_t m_Version;
	const void* m_NpCommunicationId;			//	Cast to SceNpCommunicationId* if used by plugin (defined as void* to avoid all native plugins requiring NP header file inclusion)
	const void* m_NpCommunicationSignature;		//	Cast to SceNpCommunicationSignature*
	const void* m_NpCommunicationPassphrase;	//	Cast to SceNpCommunicationPassphrase*
	bool m_NpHasTrophyPack;
	const char* m_NpServiceID;
	const char* m_TitleID;
	int m_NpAgeRating;
	bool m_Trial;
#if	defined SN_TARGET_PS3
	void* m_SpursInstance;
#endif
	bool m_bDevelopment;						//	Added in >0x0102 (TRUE in Development Build, FALSE in Non-Development)
#if __ORBIS__
	void* m_TitleSecret;
	int m_PushNotifcations;
#endif
	fnPrxPluginQueryInterface m_QueryInterface;	//	Added in >0x0103 (if available contains a function ptr to QueryInterface, otherwise NULL)
};

bool ProcessPrxPluginArgs(unsigned int sz, const void* arg, const char* pluginName);

extern fnPrxPluginQueryInterface g_QueryInterface;
extern PrxPluginArgs g_AppInfo;	// Deprecated, but we need to keep it until all versions of Unity that are out in the wild support the plugin interfaces listed above.

template<typename T> T* GetRuntimeInterface(int interfaceID)
{
	if (g_QueryInterface)
	{
		void* interfacePtr = NULL;
		int queryStatus = g_QueryInterface(interfaceID, &interfacePtr);
		if (queryStatus == PRX_PLUGIN_QUERY_SUCCESS)
		{
			return (T*)interfacePtr;
		}
	}
	return NULL;
}
