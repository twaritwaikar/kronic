# kronic

WIP Vulkan based FPS engine

# Setup

## Ubuntu Linux

1. Install dependencies

```
sudo apt-get update && sudo apt install -y vulkan-tools libvulkan-dev xorg-dev cmake build-essential
```

2. Configure CMake cache

```
mkdir build && cd build && cmake .. -DBUILD_TESTS=ON
```

3. Build kronic

```
cd build && cmake --build . --config Debug -j`grep -c '^processor' /proc/cpuinfo`
```

4. Run!

```
./build/kronic/kronic
```

> You may use the pre-defined tasks in the `.vscode/` folder to build, run, and debug easily.
