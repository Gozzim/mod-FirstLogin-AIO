# This is a module for  ![logo](https://raw.githubusercontent.com/azerothcore/azerothcore.github.io/master/images/logo-github.png) AzerothCore
## Additional First Login Features
[![core-build](https://github.com/Gozzim/mod-FirstLogin-AIO/actions/workflows/core-build.yml/badge.svg)](https://github.com/Gozzim/mod-FirstLogin-AIO)
[![CodeFactor](https://www.codefactor.io/repository/github/gozzim/mod-firstlogin-aio/badge)](https://www.codefactor.io/repository/github/gozzim/mod-firstlogin-aio)

## Description
Module to enable various features for new players

## Features

- Enable/Disable module
- Announcement of enabled module on login
- Set number of stable slots for hunters to start with
- Enable hunters starting with a random pet
    - Different options for random name generation for hunter starting pets
- Enable hunters starting with all their pet spells
- Set number of bank slots for new players
    - Different options for bags within those bank bag slots
- Enable players starting with dual spec

## Installation
```
1) Simply place the module under the `modules` directory of your AzerothCore source. 
2) Import the SQL manually to the right Database.
3) Re-run cmake and launch a clean build of AzerothCore.
```

## Edit module configuration (optional)
If you need to change the module configuration, go to your server configuration folder (where your worldserver or worldserver.exe is), copy FirstLogin.conf.dist to FirstLogin.conf and edit that new file.

## Credits
- Module for AzerothCore created by [Gozzim](https://github.com/Gozzim)
- AzerothCore: [repository](https://github.com/azerothcore) - [website](http://azerothcore.org/)

## License
This code and content is released under the [GNU AGPL license](https://github.com/Gozzim/mod-FirstLogin-AIO/blob/master/LICENSE).