#pragma once

// Plugin interface for retrieving NP data from the player.
//
// Important; to maintain compatibility with existing plugins you should not modify
// this class.
//
// If modifications or additions are required for this interface then derive a new 
// interface from this class and then add the necessary code to instantiate the new
// interface in PrxPluginInterface.cpp, PrxPluginInterface.h, and in main.cpp
//
class IPluginSceNpParams
{
public:
	virtual ~IPluginSceNpParams() {}

	// PS Vita & PS3 only. Get the applications NP communications ID.
	virtual const void* NpCommunicationsID() const = 0;

	// PS Vita & PS3 only. Get the applications NP communications signature.
	virtual const void* NpCommunicationsSignature() const = 0;

	// PS Vita & PS3 only. Get the applications NP communications pass-phrase.
	virtual const void* NpCommunicationsPassphrase() const = 0;

	// Get the applications NP service ID.
	virtual const char* NpServiceID() const = 0;

	// Get the applications NP Age rating.
	virtual int NpAgeRating() const = 0;

	// Does the application have a trophy pack included?
	virtual bool NpHasTrophyPack() const = 0;

	// PS4 only, Get the applications title secret data.
	virtual const void* NpTitleSecret() const = 0;

	// PS4 only, get the applications push notification flags.
	virtual int NpPushNotifications() const = 0;
};
