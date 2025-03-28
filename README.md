# KVP Storage Application

A key-value pair storage application for desktop and embedded Linux systems, developed as a test case for Salto CloudWorks.

## Table of Contents

- [Overview](#overview)
- [Features](#features)
- [Desktop Linux Build](#desktop-linux-build)
  - [Prerequisites](#prerequisites)
  - [Directory Structure](#directory-structure)
  - [Building](#building)
  - [Running](#running)
  - [Testing](#testing)
- [Embedded Linux Build](#embedded-linux-build)
  - [Prerequisites](#prerequisites-1)
  - [Setting Up Buildroot](#setting-up-buildroot)
  - [Creating Package for KVP Storage](#creating-package-for-kvp-storage)
  - [Building the Embedded System](#building-the-embedded-system)
  - [Running in QEMU](#running-in-qemu)
- [Usage](#usage)
- [Design Decisions](#design-decisions)
- [Possible Improvements](#possible-improvements)

## Overview

KVP Storage is an application designed to persistently store and retrieve key-value pairs. It provides a simple interface for setting, getting, and deleting key-value pairs via a command-line interface. The application is designed to run on both desktop Linux and embedded Linux systems.

## Features

- Persistent storage of key-value pairs to disk
- Thread-safe operations using mutexes
- Command-line interface with SET/GET/DELETE commands
- Extensible design to support other interfaces (e.g., IPC)
- Compatible with both desktop Linux and embedded Linux systems
- Simple file-based storage format

## Desktop Linux Build

### Prerequisites

- C++17 compatible compiler (g++ or clang++)
- CMake (version 3.10 or higher)
- Build essentials (make, etc.)

Install dependencies on Ubuntu/Debian:

```bash
sudo apt update
sudo apt install -y build-essential cmake
```

### Directory Structure

```
KVPStorage/
├── src/
│   ├── main.cpp
│   ├── storage.cpp
│   ├── storage.h
│   ├── command_processor.cpp
│   └── command_processor.h
├── tests/
│   ├── test_storage.cpp
│   └── CMakeLists.txt
├── CMakeLists.txt
├── Makefile
└── README.md
```

### Building

Using CMake (preferred for desktop development):

```bash
mkdir build
cd build
cmake ..
make
```

Using plain Makefile (compatible with Buildroot):

```bash
make
```

### Running

```bash
./KVPStorage [database_file]
```

If no database file is specified, it defaults to "kvp_storage.db" in the current directory.

### Testing

Using CMake:

```bash
cd build
make test
```

Or run the test executable directly:

```bash
./tests/test_storage
```

## Embedded Linux Build

### Prerequisites

- Linux development machine
- Buildroot (version 2023.02 or newer)
- QEMU for ARM emulation
- At least 10GB of free disk space
- Internet connection (for downloading packages)

Install dependencies on Ubuntu/Debian:

```bash
sudo apt update
sudo apt install -y build-essential wget git qemu-system-arm
```

### Setting Up Buildroot

1. **Download and extract Buildroot**:

```bash
mkdir -p ~/embedded_build
cd ~/embedded_build
wget https://buildroot.org/downloads/buildroot-2023.02.tar.gz
tar -xzf buildroot-2023.02.tar.gz
cd buildroot-2023.02
```

2. **Configure Buildroot for ARM architecture**:

```bash
make qemu_arm_vexpress_defconfig
```

### Creating Package for KVP Storage

1. **Create the package directory**:

```bash
mkdir -p package/kvp_storage
```

2. **Create Config.in file**:

```bash
cat > package/kvp_storage/Config.in << 'EOF'
config BR2_PACKAGE_KVP_STORAGE
    bool "kvp_storage"
    help
      Key-Value Pair Storage application for persistently storing and retrieving
      key-value pairs. Provides SET/GET/DELETE commands via command-line interface.
EOF
```

3. **Create kvp_storage.mk file**:

```bash
cat > package/kvp_storage/kvp_storage.mk << 'EOF'
################################################################################
#
# kvp_storage
#
################################################################################

KVP_STORAGE_VERSION = 1.0
KVP_STORAGE_SITE = $(TOPDIR)/../KVPStorage
KVP_STORAGE_SITE_METHOD = local
KVP_STORAGE_DEPENDENCIES = host-pkgconf

define KVP_STORAGE_BUILD_CMDS
    $(MAKE) CC="$(TARGET_CC)" CXX="$(TARGET_CXX)" LD="$(TARGET_LD)" -C $(@D)
endef

define KVP_STORAGE_INSTALL_TARGET_CMDS
    $(INSTALL) -D -m 0755 $(@D)/KVPStorage $(TARGET_DIR)/usr/bin/KVPStorage
endef

$(eval $(generic-package))
EOF
```

**Note**: You may need to adjust the `KVP_STORAGE_SITE` path to match the location of your KVPStorage project directory relative to the Buildroot directory.

4. **Add the package to Buildroot's package list**:

```bash
sed -i '/menu "Miscellaneous"/a\\tsource "package/kvp_storage/Config.in"' package/Config.in
```

### Building the Embedded System

1. **Configure Buildroot to include the KVP Storage package**:

```bash
make menuconfig
```

Navigate to "Package Selection for the target" -> "Miscellaneous" -> "kvp_storage" and select it by pressing the spacebar. Then save and exit.

2. **Build the system**:

```bash
make
```

This will take some time to build the entire embedded Linux system, including the kernel, root filesystem, and your KVP Storage application.

### Running in QEMU

After the build completes, run the emulator:

```bash
qemu-system-arm -M vexpress-a9 -kernel output/images/zImage -dtb output/images/vexpress-v2p-ca9.dtb -drive file=output/images/rootfs.ext2,format=raw -append "root=/dev/sda console=ttyAMA0,115200" -nographic
```

When QEMU boots, you'll be at a shell prompt. You can then run your application:

```bash
KVPStorage
```

To exit QEMU, press `Ctrl+A` followed by `x`.

## Usage

The application provides a simple command-line interface with the following commands:

### SET Command

Stores a key-value pair:

```
> SET <key> <value>
OK
```

Example:
```
> SET DeviceName TestDevice_123
OK
```

### GET Command

Retrieves a value by key:

```
> GET <key>
<value>
```

Example:
```
> GET DeviceName
TestDevice_123
```

If the key does not exist, an empty line is returned:

```
> GET NonExistentKey

```

### DELETE Command

Removes a key-value pair:

```
> DELETE <key>
OK
```

Example:
```
> DELETE DeviceName
OK
```

### EXIT Command

Exits the application:

```
> EXIT
```

## Design Decisions

- **C++17**: Modern C++ features are used to provide a clean and maintainable codebase.
- **File-based Storage**: A simple text file is used for persistent storage, with each key-value pair on a separate line in the format `key=value`.
- **Thread Safety**: Mutexes are used to ensure thread-safe operations on the storage.
- **Separation of Concerns**: The code is structured with separate classes for storage and command processing, making it easier to extend and maintain.
- **Simple Building**: Both CMake (for desktop) and a plain Makefile (for embedded) are provided for building flexibility.

