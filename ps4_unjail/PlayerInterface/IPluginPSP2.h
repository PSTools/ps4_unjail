#pragma once

// Interface for retrieving PSP2 specific data from the player.
//
// Important; to maintain compatibility with existing plugins you should not modify
// this class.
//
// If modifications or additions are required for this interface then derive a new
// interface from this class and then add the necessary code to instantiate the new
// interface in PrxPluginInterface.cpp, PrxPluginInterface.h, and in main.cpp
//
class IPluginPSP2
{
public:
	virtual ~IPluginPSP2() {}

	// Remap a Unix style path to the required PS Vita format (as expected by SCE API).
	//
	// The .NET class libs handle Unix style paths, e.g. "/mounted_device_or_root_dir/file"
	// or single character windows style device specs, e.g. "C:/somefile".
	//
	// The PS Vita device specs are a string followed by a colon, e.g. "app0:/file".
	//
	// Usage: 
	//	 buffer - a pointer to pre-allocated space large enough to hold the result.
	//	 path - a pointer to the null terminated string containing the path for conversion.
	//   size - the size of the pre allocated buffer.
	//
	// Returns: 
	//	 - If buffer is NULL returns the number of characters that would be copied to  
	//     buffer inclusive of null terminator.
	//   - If buffer is valid returns the number of characters copied.
	//   - < 0 on error.
	virtual int RemapUnixPath (char* buffer, const char* path, size_t size) const = 0;
};
