BirdWatcher for Windows
=======================

Two years ago I decided to put a camera inside a nest box.
With the analog camera I chose, I was able to see the image on my TV... very nice.

But because I could not watch my tv 24/7 and I didn't want to miss an important nest box event,
I needed the possibility to record video sequences and to watch images from anywhere.

So I bought a usb framegrabber to digitalize the signal and started to write this program.

Because I had already some background with DirectShow, I wrote it in that Microsoft environment.

It is basically able of doing the following things:
 * Record video sequences (avi compressed with your favourite codec) on local drive.
 * Record still images (jpg) on local drive.
 * Playback a sequence of recorded still images (timelaps)
 * Send jpg images on a ftp server.


Requirement to compile the program:
-----------------------------------

This program can be compiled with VisualStudio Express 2012.

You will need to install Windows SDK (version 7.1) to be able to use the BaseClasses

The base classes should be installed at the following path.
C:\Program Files\Microsoft SDKs\Windows\v7.1\Samples\multimedia\directshow\baseclasses

If it's not the case, you must modify the "Additionnal Include Directories" field in the projects' property page.

Then you also need to compile the base classes from the project located on:
C:\Program Files\Microsoft SDKs\Windows\v7.1\Samples\multimedia\directshow\baseclasses

-> this generates strmbase.lib (and strmbasd.lib in debug version)


Requirement to run the program:
-------------------------------

For the program to run correctly, bwFilter.dll and JpegReader.dll must be registered with regsvr32 program. 
This operation should be done only once.

If you need to register a new version of the bwFilter.dll or JpegReader.dll (needed if the external interface changed), 
you must first unregister them (regsvr32 /u) , then register again.


Philippe Cornet <phil.cornet@gmail.com>
