# Compiling on GNU/Linux

## Dependencies

| Dependency | Version | Commentary |
|------------|---------|------------|
| GCC        | 5.1+    | or Clang 3.5+ |
| CMake      | 3.5+    |            |
| IrrlichtMt | -       | Custom version of Irrlicht, see https://github.com/minetest/irrlicht |
| Freetype   | 2.0+    |            |
| SQLite3    | 3+      |            |
| Zstd       | 1.0+    |            |
| LuaJIT     | 2.0+    | Bundled Lua 5.1 is used if not present |
| GMP        | 5.0.0+  | Bundled mini-GMP is used if not present |
| JsonCPP    | 1.0.0+  | Bundled JsonCPP is used if not present |

For Debian/Ubuntu users:

    sudo apt install g++ make libc6-dev cmake libpng-dev libjpeg-dev libxi-dev libgl1-mesa-dev libsqlite3-dev libogg-dev libvorbis-dev libopenal-dev libcurl4-gnutls-dev libfreetype6-dev zlib1g-dev libgmp-dev libjsoncpp-dev libzstd-dev libluajit-5.1-dev

For Fedora users:

    sudo dnf install make automake gcc gcc-c++ kernel-devel cmake libcurl-devel openal-soft-devel libpng-devel libjpeg-devel libvorbis-devel libXi-devel libogg-devel freetype-devel mesa-libGL-devel zlib-devel jsoncpp-devel gmp-devel sqlite-devel luajit-devel leveldb-devel ncurses-devel spatialindex-devel libzstd-devel

For Arch users:

    sudo pacman -S base-devel libcurl-gnutls cmake libxi libpng sqlite libogg libvorbis openal freetype2 jsoncpp gmp luajit leveldb ncurses zstd

For Alpine users:

    sudo apk add build-base cmake libpng-dev jpeg-dev libxi-dev mesa-dev sqlite-dev libogg-dev libvorbis-dev openal-soft-dev curl-dev freetype-dev zlib-dev gmp-dev jsoncpp-dev luajit-dev zstd-dev

## Download

You can install Git for easily keeping your copy up to date.
If you don’t want Git, read below on how to get the source without Git.
This is an example for installing Git on Debian/Ubuntu:

    sudo apt install git

For Fedora users:

    sudo dnf install git

For Arch users:

	sudo pacman -S git

For Alpine users:

	sudo apk add git

Download source (this is the URL to the latest of source repository, which might not work at all times) using Git:

    git clone --depth 1 https://github.com/minetest/minetest.git
    cd minetest

Download Minetest Game (otherwise only the "Development Test" game is available) using Git:

    git clone --depth 1 https://github.com/minetest/minetest_game.git games/minetest_game

Download IrrlichtMt to `lib/irrlichtmt`, it will be used to satisfy the IrrlichtMt dependency that way:

    git clone --depth 1 https://github.com/minetest/irrlicht.git lib/irrlichtmt

Download source, without using Git:

    wget https://github.com/minetest/minetest/archive/master.tar.gz
    tar xf master.tar.gz
    cd minetest-master

Download Minetest Game, without using Git:

    cd games/
    wget https://github.com/minetest/minetest_game/archive/master.tar.gz
    tar xf master.tar.gz
    mv minetest_game-master minetest_game
    cd ..

Download IrrlichtMt, without using Git:

    cd lib/
    wget https://github.com/minetest/irrlicht/archive/master.tar.gz
    tar xf master.tar.gz
    mv irrlicht-master irrlichtmt
    cd ..

## Build

Build a version that runs directly from the source directory:

    cmake . -DRUN_IN_PLACE=TRUE
    make -j$(nproc)

Run it:

    ./bin/minetest

- Use `cmake . -LH` to see all CMake options and their current state.
- If you want to install it system-wide (or are making a distribution package),
  you will want to use `-DRUN_IN_PLACE=FALSE`.
- You can build a bare server by specifying `-DBUILD_SERVER=TRUE`.
- You can disable the client build by specifying `-DBUILD_CLIENT=FALSE`.
- You can select between Release and Debug build by `-DCMAKE_BUILD_TYPE=<Debug or Release>`.
  - Debug build is slower, but gives much more useful output in a debugger.
- If you build a bare server you don't need to compile IrrlichtMt, just the headers suffice.
  - In that case use `-DIRRLICHT_INCLUDE_DIR=/some/where/irrlichtmt/include`.

- Minetest will use the IrrlichtMt package that is found first, given by the following order:
  1. Specified `IRRLICHTMT_BUILD_DIR` CMake variable
  2. `${PROJECT_SOURCE_DIR}/lib/irrlichtmt` (if existent)
  3. Installation of IrrlichtMt in the system-specific library paths
  4. For server builds with disabled `BUILD_CLIENT` variable, the headers from `IRRLICHT_INCLUDE_DIR` will be used.
  - NOTE: Changing the IrrlichtMt build directory (includes system installs) requires regenerating the CMake cache (`rm CMakeCache.txt`)
