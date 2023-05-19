# Changelog
Every new MCPI++ release starting from v2.2.11 is listed here. 

- Under-the-hood modifications that don't effect the user experience are prefixed with "[INTERNAL]".

This changelog is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/) and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [2.4.7] - 2023-5-18

### Added
- Add ``Add Biome Colors To Grass`` Feature Flag (Enabled By Default)
- Add ``Generate Caves`` Feature Flag (Enabled By Default)

### Changed
- Improve Performance for Dedicated Server Software
- [INTERNAL] Allow Mods to Access Original GLFW Keycode

## [2.4.6-1] - 2023-2-14

### Fixed

- Changed PBPT default domain

## [2.4.6] - 2022-10-31

### Fixed

- Fix Crash Report Log Saving
- Fix Holding Left-Click When Attacking
- Fix Crashing On ARMHF

## [2.4.5] - 2022-10-31

### Added
- Implement Upstream Crash Dialog
- [INTERNAL] Bundle QEMU

### Removed
- Remove Coloured Terminal Output

## [2.4.4] - 2022-10-30

### Added
- Cache Previous Launcher Configuration
- Add ``MCPI_API_PORT`` Environmental Variable
- Add ``Improved Classic Title Screen`` Feature Flag (Enabled By Default)
  - Add Quit button
  - Add Options Button (Moved From ``Fix Options Screen`` Feature Flag)
- Add ``Disable Speed Bridging`` Feature Flag (Disabled By Default)
- Add ``Disable Creative Mode Mining Delay`` Feature Flag (Disabled By Default)

### Changed
- Moved ``3D Anaglyph`` Feature Flag To Options Screen
- Improved Feature Flag Namesp
- [INTERNAL] Improved Build System

### Fixed
- Fix Particles In Front-Facing View
- Fixed Launch Crash On Ubuntu ARM64
  - PatchELF Replaced With LIEF
- Miscellaneous Bug Fixes

## [2.4.3-4] - 2022-08-30

### Fixed

- Fix Ownership Issue With ``~/.minecraft-pi/`` Folder

## [2.4.3-3] - 2022-08-19
- Fix Broken DEB Signing

## [2.4.3-2] - 2022-08-19
- [INTERNAL] Changes to CMake build and dependencies

## [2.4.3-1] - 2022-08-03
- Bump Planet Version To 1.2.1
- [INTERNAL] Changes to SDK Build

## [2.4.3] - 2022-08-01
- Fix Signs With CP-437
- Fix Duplicate Creative Inventory Items
- [INTERNAL] Patch Invalid Creative Inventory Items
- Fix Picking Up Lava
- Fix Wayland App ID

## [2.4.1-1] - 2022-07-27
- Change DEB Executible Path From ``/bin/`` To ``/usr/bin/``

## [2.4.1] - 2022-07-26
- Allow More Characters In Usernames And Chat
- Fix Running On ARMHF Debian Buster
- Modding SDK
- Cache Blacklist/Whitelist
- More Reliable AppImages
- CMake Refactors
- Disable Broken Touchscreen-Specific Block Outline Behavior
- Add ``Remove Forced GUI Lag (Can Break Joining Servers)`` Feature Flag (Disabled By Default)
- Add ``Add Buckets`` Feature Flag (Enabled By Default)
- Add ``Classic HUD`` Feature Flag (Enabled By Default)
- Add ``Translucent Toolbar`` Feature Flag (Enabled By Default)
- Add ``Force EGL`` Feature Flag (Disabled By Default)
- Fix Sound Pitch/Volume/Attenuation
- Fix Holding Left-Click When Attacking
- Don't Force EGL (Should Fix Some NVIDIA Systems)
- Performance Fixes

## [2.3.13] - 2022-06-05
- Fix Texture Bug

## [2.3.12] - 2022-06-04
- Media Layer Proxy Optimizations
- Bug Fixes

## [2.3.11] - 2022-05-29
- ``--version`` Command Line Option
- TPS Measured In Benchmark & Server
- Front-Facing Third-Person
- GLESv1 Comparability Layer
- Miscellaneous Bug Fixes

## [2.3.10] - 2022-05-14
- Add Crash Report Dialog
- Disable V-Sync By Default
- Refactor Child Process Management
- Improve Build System
  - Support For Building On Ubuntu 22.04

## [2.3.9] - 2022-05-05
- Bundle An ARM Sysroot
  - Not Used On ARM32 Systems
  - Based On Debian Bullseye
- Colored Log Output

## [2.3.8] - 2022-05-01
- Switch Up Mod Loading Order

## [2.3.7] - 2022-04-28
- Don't Append Hyphens To New World Name, Only Folder Names

## [2.3.6] - 2022-04-27
- Fix ``Invert Y-axis`` Option Name
- Improve Touch GUI Inventory In Non-Touch GUI
- New Create World Dialog
  - Controlled By ``Implement Create World Dialog`` Feature Flag (Enabled By Default)
  - Custom World Names
  - Game-Mode Selection
  - Custom Seeds

## [2.3.5] - 2022-04-23
- Renamed Some Feature Flags
- Add ``Improved Title Background`` Feature Flag (Enabled By Default)
- Non-Touch GUI Rework
  - Make ``Full Touch GUI`` Feature Flag Disabled By Default
  - Add ``Force Touch GUI Button Behavior`` Feature Flag (Enabled By Default)
  - Add ``Improved Button Hover Behavior`` Feature Flag (Enabled By Default)

## [2.3.4] - 2022-04-20
- AppImage Fixes
- Make Death Messages Customizable Server-Side
- Fix Q-Key Behavior Behavior When Editing Signs
- Add ``Force Touch Inventory`` Feature Flag (Disabled By Default)
- Add ``Fix Pause Menu`` Feature Flag (Enabled By Default)
  - Enables Server Visibility Toggle Button
- Options Changes (Not Supported On Legacy)
  - Add ``Fix Options Screen`` Feature Flag (Enabled By Default)
    - Adds Options Button To Classic UI Start Screen
    - Removes Useless Options Toggles
    - Fixes Options Toggles' Default Position
  - Store Multiple Settings In `options.txt`
    - ``Peaceful Mode`` Feature Flag Moved To ``game_difficulty``
    - ``Smooth Lighting`` Feature Flag Moved To ``gfx_ao``
    - ``Fancy Graphics`` Feature Flag Moved To ``gfx_fancygraphics``
    - ``Disable Hosting LAN Worlds`` Feature Flag Moved To ``mp_server_visible_default``

## [2.3.3] - 2022-04-17
- Add More Blocks To Expanded Creative Inventory
- Add AppStream Metadata

## [2.3.2] - 2022-03-23
- Simplify Launch Sequence
- Add More Blocks To Expanded Creative Inventory
- Fix Nether Reactor With Creative Restrictions Disabled
- Alphabetize Feature Flags
- Add ``Disable V-Sync`` Feature Flag (Disabled By Default)
- INTERNALernal Refactor Of ``libreborn``
- Remove Use Of ``/bin/sh``
- Load Custom Mods First
- Use Zenity Dark Mode
- Add ``Improved Cursor Rendering`` Feature Flag (Enabled By Default)

## [2.3.0] - 2022-03-12
- Switch To AppImage For Packaging
- Prevent OpenAL From Crashing When Out Of Memory
- Vendor GLFW & Zenity
  - Seamless Wayland Support
- Add ``MCPI_DEBUG`` Environmental Variable
- Add ``Disable Hosting LAN Worlds`` Feature Flag (Disabled By Default)
- Add ``Fix Furnace Not Checking Item Auxiliary`` Feature Flag (Enabled By Default)
- Add ``Disable Raw Mouse Motion (Not Recommended)`` Feature Flag (Disabled By Default) For Broken X11 Setups
- Added Back `~/.minecraft-pi/mods`
- Improve Build System
- Improve Documentation

## [2.2.11] - 2022-02-27
- Add ``Close Current Screen On Death`` Feature Flag (Enabled By Default) To Prevent Bugs
- Fix More Furnace UI Bugs When Using "Disable 'gui_blocks' Atlas"
