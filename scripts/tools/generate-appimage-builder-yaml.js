#!/usr/bin/env node

// Child Process
const child_process = require('child_process');

// Arguments
if (process.argv.length < 4) {
    throw new Error('Invalid Arguments');
}
const mode = process.argv[2];
const arch = process.argv[3];

// Version
const fs = require('fs');
const version = fs.readFileSync('VERSION', 'utf8').trim();

// Data
const id = `com.thebrokenrail.MCPIReborn${mode === 'server' ? 'Server' : 'Client'}`;
const name = `minecraft-pi-reborn-${mode}`;

// APT Sources
const apt_sources = child_process.execFileSync('./scripts/tools/get-apt-sources.sh', [], {encoding: 'utf-8'}).trim().split('\n');
const apt_sources_formatted = [];
for (const apt_source of apt_sources) {
    apt_sources_formatted.push({sourceline: apt_source});
}

// Packages/Dependencies
const packages = [
    'libstdc++6',
    'patchelf'
];
if (mode === 'client') {
    // GLFW's Dependencies Aren't Included As They Should Be Provided By The Host System
    packages.push(
        'libgtk-3-0',
        'libglib2.0-0',
        'libgdk-pixbuf2.0-0',
        'librsvg2-common',
        'shared-mime-info',
        'libfreeimage3',
        'libopenal1'
    );
}
if (arch !== 'armhf' && arch !== 'arm64') {
    packages.push('qemu-user');
}

// Package Exclusions
const packageExclusions = [
    // Exclude Unneeded Packages
    'humanity-icon-theme',
    'adwaita-icon-theme',
    'libxml2',
    '*systemd*',
    'dconf-service',
    'dconf-gsettings-backend',
    'librest-*',
    'libcups2',
    'libcolord2',
    'libmount1',
    'libwayland-*'
];

// APT
const apt = {
    arch: arch,
    sources: apt_sources_formatted,
    allow_unauthenticated: true,
    include: packages,
    exclude: packageExclusions
};

// Get Architecture Triplet
const triplet = {
    'amd64': 'x86_64-linux-gnu',
    'i386': 'i386-linux-gnu',
    'arm64': 'aarch64-linux-gnu',
    'armhf': 'arm-linux-gnueabihf'
}[arch];
if (!triplet) {
    throw new Error();
}

// Files
const files = {
    exclude: [
        // Exclude Unused Files
        `usr/lib/${triplet}/gconv`,
        'usr/share/man',
        'usr/share/doc/*/README.*',
        'usr/share/doc/*/changelog.*',
        'usr/share/doc/*/NEWS.*',
        'usr/share/doc/*/TODO.*',
        'usr/include',
        'usr/share/lintian',
        'usr/share/gcc',
        'usr/share/gdb',
        'usr/share/locale',
        'usr/share/help',
        'usr/bin/update-mime-database'
    ]
};

// Script After Bundling Dependencies
const afterBundle = [
    // Remove Unused QEMU Binaries
    'find ./AppDir/usr/bin -maxdepth 1 -name \'qemu-*\' -a ! -name \'qemu-arm\' -delete'
];

// Environment
const env = {
    APPDIR_MODULE_DIR: '/tmp/.minecraft-pi-patched'
};
if (mode === 'client') {
    // Make GTK Work (Zenity Uses GTK)
    env.GTK_EXE_PREFIX = '${APPDIR}/usr';
    env.GTK_PATH = `\${APPDIR}/usr/lib/${triplet}/gtk-3.0`;
    env.GTK_DATA_PREFIX = '${APPDIR}';
    env.GTK_THEME = 'Default';
    env.XDG_DATA_DIRS = '${APPDIR}/share:${APPDIR}/usr/share:/share:/usr/share';
    env.APPDIR_LIBRARY_PATH = `\${APPDIR}/usr/lib/${triplet}:\${APPDIR}/usr/${triplet}/lib:\${APPDIR}/lib/${triplet}:\${APPDIR}/usr/lib:\${APPDIR}/usr/lib/${triplet}/gdk-pixbuf-2.0/2.10.0/loaders`;
}

// Runtime
const runtime = {
    env: env,
    preserve: arch !== 'armhf' ? [
        // On non-ARM32 systems, an ARM32 linker is embedded, this
        // prevents AppImage-Builder from modifying ARM32 binaries
        // to use a (usually non-existent) system linker.
        `usr/lib/${name}/minecraft-pi`,
        `usr/lib/${name}/**/*.so`,
        `usr/lib/${name}/sysroot`
    ] : [
        // MCPI's license prohibits distributing a modified
        // minecraft-pi binary.
        `usr/lib/${name}/minecraft-pi`
    ]
};

// AppDir
const appDir = {
    path: `./AppDir`,
    app_info: {
        id: id,
        name: `${name}`,
        icon: id,
        version: version,
        exec: `usr/bin/${name}`,
        exec_args: '$@'
    },
    apt: apt,
    files: files,
    after_bundle: afterBundle,
    runtime: runtime
};

// Build Script
const script = [
    `rm -rf ./build/${mode}-${arch}`,
    `./scripts/setup.sh ${mode} ${arch} -DMCPI_IS_APPIMAGE_BUILD=ON`,
    `rm -rf ./out/${mode}-${arch}`,
    `./scripts/build.sh ${mode} ${arch}`,
    'rm -rf ./AppDir',
    `cp -ar ./out/${mode}-${arch} ./AppDir`
];

// AppImage
const appImageArch = {
    'amd64': 'x86_64',
    'i386': 'i686',
    'arm64': 'aarch64',
    'armhf': 'armhf'
}[arch];
if (!appImageArch) {
    throw new Error();
}
const appImage = {
    arch: appImageArch,
    file_name: `./out/${name}-${version}-${arch}.AppImage`,
    'update-information': `gh-releases-zsync|mobilegmYT|mcpi-reborn-extended|latest|${name}-${arch}.AppImage.zsync`
};

// Root
const root = {
    version: 1,
    AppDir: appDir,
    script: script,
    AppImage: appImage
};

// Write
fs.writeFileSync(`AppImageBuilder.yml`, JSON.stringify(root, null, 4));