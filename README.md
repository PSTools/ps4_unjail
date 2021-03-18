# ps4_unjail
![image](https://i.imgur.com/qTGhdSY.png)

PS4 Universal Plugin
Orhinally by @zecoxao


### Using Universal

To start you will need to do a DLL Import in unity (universal.prx needs to be placed inside Assets\Plugins\PS4)

#### DLL Import
```c#
    [DllImport("universal")]
    //Custom Funciton to be added like below
    private static extern UInt16 get_firmware();
```



#### Unjail
```c#
   private static extern int FreeUnjail(int FWVersion);
   //Will unjail the current process (you're game or app)
   //can be combined with the GetFirmware funciton to auto unjail 
   FreeUnjail(get_firmware());
```

#### Temperature
```c#
   private static extern int Temperature();
   //Will return temp in ºC
```

#### FreeFTP
```c#
   private static extern int FreeFTP();
   //Will enable FTP on the console
```

#### FreeMount
```c#
   private static extern int FreeMount();
   //Allows full rw
```

### SetTemperature
```c#
    private static extern void SetTemperature(int celsius);
    //allows you to set the temprature

```
# Notifications 
<details>
  <summary>Click to expand</summary>
    
### Notification on PS4
```c#
    private static extern int SendMessageToPS4(string Message)
    //Shows a notification on the PS4
```

</details>

# Save Data
<details>
  <summary>Methods that can be used for save data click to expand</summary>
  
 ### MountSaveData
```c#
    private static extern int MountSaveData(string TITLEID,string fingerprint)
    //mounts save data on the PS4 if using save mounter patches please use zero's for fingerprint
```

### UnMountSaveData
```c#
    private static extern int UnMountSaveData()
    //Unmounts all mounted save data
```
</details>

# Kernel Calls
<details>
  <summary>Click to expand</summary>
  
  ## Basic Kernel Calls
 ### Load another executable
```c#
    private static extern bool LoadExec(string path, string argv)
    //Calls and opens an application
```
### GetIDPS
```c#
    private static extern string GetIDPS()
    //Gives you you're IDPS
```
### GetPSID
```c#
    private static extern string GetPSID()
    //Gives you you're PSID
```
#### Get_Firmware this will get the current firmware of the console not the spoofed firmware
```c#
   private static extern UInt16 get_firmware();
   // should return as XXX e.g 505, 702 or 755
```
### get_fw
```c#
    private static extern int get_fw()
    //gets the version as an int (reads from kern.sdk_version) can be spoofed
```

 ## Experimental Calls
### GetCallableList
```c#
    private static extern string GetCallableList()
    //Designed to show you all items you can call on the syste,
```
### GetListOfServices
```c#
    private static extern string GetListOfServices()
    //Designed to show you all services you can call on the syste,
```
### KernelGetOpenPsId
```c#
    private static extern string KernelGetOpenPsId()
    //Not sure why this call exists
```
### firmware_version_kernel
```c#
    private static extern string firmware_version_kernel()
    //Get the firmware version on the kernel (but can be spoofed !)
```
### firmware_version_libc
```c#
    private static extern string firmware_version_kernel()
    //Get the firmware version by libc (for prevent from kernel spoof)
    //Should no longer be required thanks to LM
```


</details>


# User Service Functions
<details>
  <summary>Click to expand</summary>
  
 ### GetUsername
```c#
    private static extern string GetUsername()
    //returns the current username
```

### GetUserId
```c#
    private static extern string GetUserId()
    //returns the current userid
```
</details>


# Trophies
<details>
  <summary>Click to expand</summary>
    
 ## Basic Calling Method
    
 ### UnlockTrophies
```c#
    private static extern int UnlockTrophies(string TitleId,string Titleidsecret )
    //returns the current username
```
## Experimental
### MakeCusaAppReadWrite
```c#
    private static extern int MakeCusaAppReadWrite()
    //returns the current userid
```
</details>

# Controler
<details>
  <summary>Click to expand</summary>
    
 ## Basic Calling Method
    
 ### Change_Controller_Color
```c#
    private static extern int Change_Controller_Color(int r,int g,int b)
    //Changes controler collor to RBG provided
```
</details>

# Sceenshot
<details>
  <summary>Click to expand</summary>
    
 ## Basic Calling Method
    
 ### TakeScreenShot
```c#
    private static extern int TakeScreenShot()
    //Should take a screenshot of the current screen
```
</details>

# Mount
<details>
  <summary>Experimental Click to expand</summary>
    
 ## Experimental
    
 ### MountandLoad
```c#
    private static extern int MountandLoad()
    //try's to mount something in sandbox
```
## Experimental
### MountTrophy
```c#
    private static extern int MountTrophy()
    //try's to mount a trophy file
```
</details>



