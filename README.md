# This is a module for  ![logo](https://raw.githubusercontent.com/azerothcore/azerothcore.github.io/master/images/logo-github.png) AzerothCore
## Additional First Login Features
Latest build status with azerothcore: [![core-build](https://github.com/Gozzim/mod-first-login/actions/workflows/core-build.yml/badge.svg)](https://github.com/Gozzim/mod-first-login)

## Description
Module to enable various features for new players

## Features

- Enable/Disable module
- Announcement of enabled module on login
- Enable hunters starting with all 4 stable slots
- Enable hunters starting with a random pet
    - Different options for random name generation for hunter starting pets
- Enable players starting with all bank slots
    - Different options for bags within those bank bag slots
- Enable players starting with dual spec

## Installation
```
1) Simply place the module under the `modules` directory of your AzerothCore source. 
2) Import the SQL manually to the right Database.
3) Re-run cmake and launch a clean build of AzerothCore.
```

## Edit module configuration (optional)
If you need to change the module configuration, go to your server configuration folder (where your worldserver or worldserver.exe is), copy first-login.conf.dist to first-login.conf and edit that new file.


## Credits
Module for AzerothCore created by [Gozzim](https://github.com/Gozzim)

#### Additional Credits
AzerothCore: [repository](https://github.com/azerothcore) - [website](http://azerothcore.org/)

## License
This code and content is released under the [GNU AGPL license](https://github.com/Gozzim/mod-first-login/blob/master/LICENSE).