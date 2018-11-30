# MacPlayer
A Spotify Connect client for 68k Macs.

## Building from Source
MacPlayer requires Retro68 for compilation, and the following libraries:

* [MacWifiLib](https://github.com/antscode/MacWifi)
* [MacAuthLib](https://github.com/antscode/MacAuthLib)
* [gason++](https://github.com/antscode/gason--)

Once the above libraries are built and installed, execute these commands from the top level of the MacPlayer directory:

    cd ..
    mkdir MacPlayer-build
    cd MacPlayer-build
    cmake ../MacPlayer -DCMAKE_TOOLCHAIN_FILE=<<YOUR_PATH_TO_Retro68-build>>/toolchain/m68k-apple-macos/cmake/retro68.toolchain.cmake
    make

## Credits
* MacPlayer is built using [Retro68](https://github.com/autc04/Retro68).
* Cursive font in logo is [streamster](https://befonts.com/streamster-font.html) by Youssef Habchi