#pragma once

// Plugin interface for retrieving application data from the player, e.g. things that relate
// to a Sony application's param.sfo file
//
// Important; to maintain compatibility with existing plugins you should not modify
// this class.
//
// If modifications or additions are required for this interface then derive a new 
// interface from this class and then add the necessary code to instantiate the new
// interface in PrxPluginInterface.cpp, PrxPluginInterface.h, and in main.cpp
//
class IPluginSceAppParams
{
public:
	virtual ~IPluginSceAppParams() {}

	// Is the application upgradeable, e.g. a trial app.
	virtual bool IsTrialApp() const = 0;

	// Get the applications SCE service ID.
	virtual const char* ServiceID() const = 0;

	// Get the applications SCE title ID.
	virtual const char* TitleID() const = 0;
};
