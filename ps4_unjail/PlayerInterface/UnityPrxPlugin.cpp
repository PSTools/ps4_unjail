#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string>
#include <assert.h>
#define Assert assert
#include "UnityPrxPlugin.h"

fnPrxPluginQueryInterface g_QueryInterface = NULL;
PrxPluginArgs g_AppInfo;	// DEPRECATED, but we need to keep it until all versions of Unity that are out in the wild support the plugin interfaces.

bool ProcessPrxPluginArgs(unsigned int sz, const void* arg, const char* pluginName)
{
	if (PrxPluginArgsV2::Validate((PrxPluginArgsV2*)arg, sz))
	{
		g_QueryInterface = ((PrxPluginArgsV2*)arg)->m_QueryInterface;
	}
	else if (PrxPluginArgs::Validate((PrxPluginArgs*)arg))
	{
		// DEPRECATED, but we need to keep it until all versions of Unity that are out in the wild support the plugin interfaces.
		g_QueryInterface = ((PrxPluginArgs*)arg)->m_QueryInterface;
		g_AppInfo = *(PrxPluginArgs*)arg;
	}
	else
	{
		printf("\nERROR...\n");
		printf(" %s is an old version that cannot be used by the current player runtime.\n", pluginName);
		printf(" Please update the %s native module and any associated managed assemblies to the latest versions\n", pluginName);
		printf("  Plugin args version, found %04x, expected %04x\n", ((PrxPluginArgsV2*)arg)->m_Version, ((PrxPluginArgsV2*)arg)->s_Version);
		printf("  Plugin args struct size, expected %ld or %ld, found %d\n\n", sizeof(PrxPluginArgs), sizeof(PrxPluginArgsV2), ((PrxPluginArgsV2*)arg)->m_Size);

		return false;
	}

	return true;
}
