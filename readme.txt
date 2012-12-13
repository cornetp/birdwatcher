Requirement:

You will need to install Windows SDK (version 7.1) to be able to use the BaseClasses

The base classes should be installed at the following path.
C:\Program Files\Microsoft SDKs\Windows\v7.1\Samples\multimedia\directshow\baseclasses

If it's not the case, you must modify the "Additionnal Include Directories" field in the projects' property page.

Then you also need to compile the base classes from the project located on:
C:\Program Files\Microsoft SDKs\Windows\v7.1\Samples\multimedia\directshow\baseclasses

-> this generates strmbase.lib (and strmbasd.lib in debug version)


Once compiled the dll bwFilter.dll must be registered with regsvr32 program. This operation should be done only once.
If you need to register a new version of the bwFilter.dll (needed if the external interface changed), you must first unregister it (regsvr32 /u) then register it again.