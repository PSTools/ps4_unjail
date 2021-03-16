#pragma once

// Interface for retrieving PS3 specific data from the player.
//
// Important; to maintain compatibility with existing plugins you should not modify
// this class.
//
// If modifications or additions are required for this interface then derive a new 
// interface from this class and then add the necessary code to instantiate the new
// interface in PrxPluginInterface.cpp, PrxPluginInterface.h, and in main.cpp
//
class IPluginPS3
{
public:
	virtual ~IPluginPS3() {}

	// Get the PS3 spurs instance.
	virtual void* GetPS3SpursInstance() const = 0;
};
