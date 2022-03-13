# Command Line Usage

## Command Line Arguments

### ``--print-available-feature-flags`` (Client Mode Only)
If you run MCPI-Reborn with ``--print-available-feature-flags``, it will print the available feature flags and then immediately exit.

The feature flags are printed in the following format:
```
TRUE This Flag Is On By Default
FALSE This Flag Is Off By Default
```

### ``--only-generate`` (Server Mode Only)
If you run MCPI-Reborn with ``--only-generate``, it will immediately exit once world generation has completed. This is mainly used for testing if the game works during the automatic build process.

### ``--benchmark`` (Client Mode Only)
If you run MCPI-Reborn with ``--benchmark``, it will enter a simple benchmark mode. This means automatically loading a newly generated world, then rotating the camera for a period of time. When it has finished, it will then exit and print the average FPS while the world was loaded. In this mode, all user input is blocked. However you can still modify rendering settings by changing feature flags.

The world used will always be re-created on start and uses a hard-coded seed.

## Environmental Variables

### ``MCPI_DEBUG``
This enables debug logging if you set it to any non-zero-length value.

### Client Mode Only
If a value isn't set for any of the following variables, a GUI will open that allows you to select one.

### ``MCPI_FEATURE_FLAGS``
This corresponds to ``--print-available-feature-flags``. This is just a list of all enabled feature flags separated by ``|``.
For instance, the string ``Feature A|Feature B`` would enable both ``Feature A`` and ``Feature B`` and *disable every other available feature flag*.

### ``MCPI_RENDER_DISTANCE``
This is the render distance. The possible values are: ``Far``, ``Normal``, ``Short``, and ``Tiny``.

### ``MCPI_USERNAME``
This is the username.

## Launching with environment variables (Client Mode Only)
If you want to launch MCPI-Reborn directly without the launcher you can use environment variables to skip it.

Here are avilable variables:
- MCPI\_FEATURE\_FLAGS A pipe ("|") separated list of enabled features. (Don't include disabled features)
- MCPI\_RENDER\_DISTENCE Can be set to "Far", "Normal", "Short", or "Tiny".
- MCPI_USERNAME Your players username
- MCPI\_SPEED\_HACK (MCPI++ Only) How much speed is increased by the speed hack (Screen flips over 3)

Here is an example:
```bash
export MCPI_FEATURE_FLAGS = 'Touch GUI|Fix Bow & Arrow|Fix Attacking|Fancy Graphics|Disable Autojump By Default|Display Nametags By Default|Fix Sign Placement|Show Block Outlines|Expand Creative Inventory|Remove Creative Mode Restrictions|Animated Water|Remove Invalid Item Background|Disable "gui_blocks" Atlas|Smooth Lighting|Fix Camera Rendering|Implement Chat|Implement Death Messages|Implement Game-Mode Switching|Allow Joining Survival Servers|Miscellaneous Input Fixes|Bind "Q" Key To Item Dropping|Bind Common Toggleable Options To Function Keys|Render Selected Item Text|External Server Support|Load Language Files|Implement Sound Engine|Hide Death Messages'
export MCPI_RENDER_DISTANCE='Normal'
export MCPI_USERNAME = 'AlexPi'
export MCPI_SPEED_HACK = 3
minecraft-pi-reborn-client
```

