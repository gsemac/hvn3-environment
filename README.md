## hvn3-environment

This is a standalone component of [hvn3-engine](https://github.com/gsemac/hvn3-engine) intended for getting information about the runtime environment of the program.

#### OperatingSystemDefs.h

`OperatingSystemDefs.h` defines `HVN3_OS_WINDOWS` on Windows, `HVN3_OS_MACOS` on MacOS, `HVN3_OS_LINUX` on Linux, `HVN3_OS_UNIX` on Unix, and `HVN3_OS_FREEBSD` on FreeBSD, providing an easy way to determine the current operating system without having to check for various standard and non-standard definitions.

It also defines `HVN3_OS_X64` on 64-bit operating systems, and `HVN3_OS_X86` on 32-bit operating systems. To detect Unix-based operating systems generically, `HVN3_OS_UNIX_BASED` is also defined on Unix-based platforms.

#### Environment.h

`Environment.h` offers a cross-platform interface for getting information about the runtime environment, including the current user's user name, the number of processors on the current machine, and the amount of memory mapped to the current process.

#### OperatingSystem.h

`OperatingSystem.h` contains declarations for platform enumeration types and the `PlatformInfo` class. The `OperatingSystemInfo` function returns a `PlatformInfo` instance containing information such as the current platform, platform name, and platform version.