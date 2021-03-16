#pragma once

// Plugin interface for retrieving general Unity information from the player.
//
// Important; to maintain compatibility with existing plugins you should not modify
// this class.
//
// If modifications or additions are required for this interface then derive a new 
// interface from this class and then add the necessary code to instantiate the new
// interface in PrxPluginInterface.cpp, PrxPluginInterface.h, and in main.cpp
//
class IPluginUnity
{
public:
	virtual ~IPluginUnity() {}

	// Unity version as an integer.
	virtual int UnityIntVersion() const = 0;

	// Unity version as a string.
	virtual const char* UnityVersion() const = 0;

	// Is this a development build?
	virtual bool IsDevelopmentBuild() const = 0;

	// Is this a debug build? This is always true unless building the Unity player from source with configuration set to 'debug'.
	virtual bool IsDebugBuild() const = 0;
};
