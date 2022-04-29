# Sound Compilation
If you have found this, you are probably big brain person looking through the code. You should consider joining the MCPI Revival dev team!!

MCPI++'s sound support comes from it's preincluded sound library, which is essentially just a big list of sound names and a corresponding sound, defined in numerial format.
This is a propreitary format inherited from reborn, and I have no idea how it works.

# Extracting sound
You will need to get the `libminecraftpe.so` file from a MCPE 0.6.1/0.8.1 APK. Once you have it, you can compile the sound extractor with clang:

```
clang++ -o extract ./extract.cpp
```

Now, you can run the extractor to generate a source file with the sound definitions:

```
./extract [path To libminecraftpe.so] > minecraftpe.c
```

# Compiling sound

You need to install depends:

```
sudo apt-get install libc6-armel-cross libc6-dev-armel-cross binutils-arm-linux-gnueabi libncurses5-dev build-essential
```

and then compile the sound source definitions:

```
arm-linux-gnueabihf-gcc -shared -o libminecraftpe.so minecraftpe.c 
```