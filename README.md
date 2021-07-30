# ps2quake
Nicolas Plourde has ported the classic game Quake to the Playstation 2 console.
The game is now fully playable but without sound.
You will need a usb keyboard and mouse supported by the ps2.
You will need either the shareware or full retail of Quake for the Data files to play this port of Quake.



Hi, I just tried to upload to ps2dev.org the first draft of my quake 1 port for the ps2. I was not able to upload the file for some reason (maybe the script dont like Mac OS X + firefox), anyway can I mail it to someone who can upload it to ps2dev.org.

About the port. I only support the video output module yet, no sound, no control and no network. Quake look for id1 directory in host: and is not able to read file from .pak it seem so you have to uncompress it before (maybe its just me so try first). I also have to find a way to load it from hdd or cdrom since network transfer is really slow. Next on my todo list is also to optimise quake math function to run on the vu and port the software drawing engine to use harware accel, right now its running in software at about 10fps, not bad at all. All ps2 code is based on Dreamtime Tutorial.
