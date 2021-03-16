#pragma once
// THIS FILE IS SHARED WITH THE PLAYER AND OTHER PLUGINS

#include <stdint.h>

// ============================================================
// Query interface return codes 
#define PRX_PLUGIN_QUERY_SUCCESS				0x00000000
#define PRX_PLUGIN_QUERY_NOT_READY				0x00000001

#define PRX_PLUGIN_QUERY_ERR_BAD_ARGS			0x80000001
#define PRX_PLUGIN_QUERY_ERR_NOT_SUPPORTED		0x80000002

// ============================================================
// Function pointer typedef for plugin query interface
typedef int(*fnPrxPluginQueryInterface)(int, void**);

// Application data passed from the player.
//
//  The only data passed through this structure is a pointer to the PrxPluginQueryInterface function in
//  the player runtime, all other data is obtained by calling into the runtime using the appropriate
//  interface from the runtime.
struct PrxPluginArgsV2
{
	static bool Validate(PrxPluginArgsV2* arg, size_t argSize)
	{
		if( (argSize < sizeof(PrxPluginArgsV2)) || (arg->m_Size != sizeof(PrxPluginArgsV2)) || (arg->m_Version != s_Version) )
		{
			return false;
		}

		return true;
	}

	PrxPluginArgsV2() : m_Size(0) ,m_Version(0), m_QueryInterface(NULL) {}
	static const uint32_t s_Version = 0x0200;	// Version number.
	static const size_t s_LegacySize = 52;		// Size of the old PrxPluginArgs struct.
	uint32_t m_Size;							// Size of this structure, for validating the structure when the plugin starts.
	uint32_t m_Version;							// Structure version number, for validating the structure when the plugin starts.
	fnPrxPluginQueryInterface m_QueryInterface;	// Pointer to the runtime interface query function.
};

// ============================================================
// Interface IDs

#define PRX_PLUGIN_IFACE_ID_GLOBAL_EVENT_QUEUE		0x00000000
#define PRX_PLUGIN_IFACE_ID_UNITY					0x00000001
#define PRX_PLUGIN_IFACE_ID_SCE_APP_PARAMS			0x00000002
#define PRX_PLUGIN_IFACE_ID_SCE_NP_PARAMS			0x00000003
#define PRX_PLUGIN_IFACE_ID_PS3						0x00000004
#define PRX_PLUGIN_IFACE_ID_PS4						0x00000005
#define PRX_PLUGIN_IFACE_ID_PSP2					0x00000006

