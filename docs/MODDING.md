# Modding
Modding Minecraft: Pi Edition is possible by patching the binary at runtime. To make this easier ``minecraft-pi-reborn`` includes a library called ``libreborn.so`` which provides several functions to help you patch the game.

## Hex Addresses
Minecraft: Pi Edition has no symbols so you must patch the hex address of an instruction instead of using a function name. 
Hex addresses can be found using tools like [Ghidra](https://ghidra-sre.org) or [RetDec](https://retdec.com). 
To find out what a function does, you can find its equivalent in [MCPE 0.6.1](https://archive.org/download/MinecraftPE0.1.0/Minecraft%20PE%200.6.1.apk) and use its name for reference because it includes [export symbols](https://stackoverflow.com/a/10903784/14798541).

## Loading Directories
``minecraft-pi-reborn`` loads mods from `/usr/lib/minecraft-pi-reborn-client/mods`.

## C++ Strings
Minecraft: Pi Edition was compiled with an old version of GCC, so when interacting with C++ strings, make sure you set ``-D_GLIBCXX_USE_CXX11_ABI=0``.

## ``__attribute__((constructor)) static void init() { }``
You will see this a lot in the docs, it is the MCPI-Reborn equivlent of `int main() {}`.
When a mod is loaded the code inside of it will be executed, for example:
```cpp
#include <stdio.h>

__attribute__((constructor)) static void init() {
    printf("Helloworld\n");
}
```
This code will print "Helloworld" when loaded by MCPI-Reborn.

## ``libreborn.so`` API
Header files and the shared library can be download from [Jenkins](https://jenkins.thebrokenrail.com/job/minecraft-pi-reborn/job/master/lastSuccessfulBuild/artifact/out/lib).

### ``void overwrite(void *start, void *target)``
This method replaces a function with another function.

#### Parameters
- **start:** The function you are replacing.
- **target:** The function you are replacing it with.

#### Return Value
None

#### Warning
This should never be used on functions that are only 1 byte long because it overwrites 2 bytes.

#### Example
```c
static int func_injection(int a, int b) {
    return a + 4;
}

__attribute__((constructor)) static void init() {
    overwrite((void *) 0xabcde, func_injection);
}
```

### ``void overwrite_call(void *start, void *original)``
This allows you to overwrite a specific call of a function rather than the function itself. This allows you to call the original function. However, this does not effect VTables.

#### Parameters
- **start:** The address of the function call to overwrite.
- **target:** The function call you are replacing it with.

#### Return Value
None

#### Warning
This method can only be safely used 512 times in total.

#### Example
```c
typedef int (*func_t)(int a, int b);
static func_t func = (func_t) 0xabcde;
static void *func_original = NULL;

static int func_injection(int a, int b) {
    (*func)(a, b);

    return a + 4;
}

__attribute__((constructor)) static void init() {
    overwrite_call((void *) 0xabcd, func_injection);
}
```

### ``void overwrite_calls(void *start, void *original)``
This allows you to overwrite all calls of a function rather than the function itself. This allows you to call the original function. However, this does not effect VTables.

#### Parameters
- **start:** The function call to overwrite;
- **target:** The function call you are replacing it with.

#### Return Value
None

#### Warning
This method can only be safely used 512 times in total.

#### Example
```c
typedef int (*func_t)(int a, int b);
static func_t func = (func_t) 0xabcde;
static void *func_original = NULL;

static int func_injection(int a, int b) {
    (*func)(a, b);

    return a + 4;
}

__attribute__((constructor)) static void init() {
    overwrite_calls((void *) func, func_injection);
}
```

### ``void patch(void *start, unsigned char patch[])``
This allows you to replace a specific instruction.

#### Parameters
- **start:** The target instruction.
- **patch:** The new instruction (array length must be 4).

#### Return Value
None

#### Example
```c
__attribute__((constructor)) static void init() {
    unsigned char patch_data[4] = {0x00, 0x00, 0x00, 0x00};
    patch((void *) 0xabcde, patch_data);
}
```

## Interacting with user options.
You may want a mod to only ativate if the user has selected a certain options.
This can be achived by including the feature mod and using the `feature_has` function.
It returns a bool.
Here is an example:
```c
chat_enabled = feature_has("Implement Chat", 1);
```
## Using custom/modified mods
To compile your mod use the following code:
```bash
arm-linux-gnueabihf-g++ -shared -fPIC -DREBORN_HAS_COMPILED_CODE <MODFILE>.cpp -o lib<MODFILE>.so
sudo chmod 644 lib<MODFILE>.so
sudo chown root lib<MODFILE>.so
sudo chgrp root lib<MODFILE>.so
```
To load your mod you will need to move it to the mod folder throught the following snippet:
```bash
sudo mv lib<MODFILE>.so /usr/lib/minecraft-pi-reborn-client/mods/lib<MODFILE>.so
```
For both, you will need to replace "\<MODFILE>" with the name of the file you made.
If you are making a server-side mod instead of a client-side mod you will need to change `minecraft-pi-reborn-client` to `minecraft-pi-reborn-server`.
