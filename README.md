# ps4_unjail
![image](https://i.imgur.com/qTGhdSY.png)

 PS4 Universal Plugin


### Using Universal

To start you will need to do a DLL Import in unity (universal.prx needs to be placed inside Assets\Plugins\PS4)

#### DLL Import
```c#
    [DllImport("universal")]
    //Custom Funciton to be added like below
    private static extern UInt16 get_firmware();
```

#### Get_Firmware this will get the current firmware of the console not the spoofed firmware
```c#
   private static extern UInt16 get_firmware();
   // should return as XXX e.g 505, 702 or 755
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

