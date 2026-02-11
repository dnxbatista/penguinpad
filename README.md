# PenguinPad

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![C++20](https://img.shields.io/badge/C++-20-blue.svg)](https://isocpp.org/)
[![SDL3](https://img.shields.io/badge/SDL-3-green.svg)](https://www.libsdl.org/)

A lightweight and efficient tool to test gamepad inputs, developed in C++.

![PenguinPad Screenshot](assets/images/screenshot.png)
*Replace with actual screenshot*

## Features

- ✅ **Real-time Detection** - Instantly identifies when a controller is connected or disconnected
- 🎮 **Visual Mapping** - Visualize buttons, analog sticks (axes), and triggers through an intuitive UI
- ⚡ **SDL3 Support** - Leverages the latest hardware management improvements from SDL3
- 🚀 **High Performance** - Minimal resource footprint thanks to ImGui's immediate mode rendering
- 🖥️ **Cross-Platform** - Works on Windows, Linux, and macOS

## Getting Started

### Prerequisites

Ensure you have the following installed on your system:
- **C++ Compiler**: GCC 11+, Clang 14+, or MSVC 2019+
- **CMake**: Version 3.24 or higher
- **Git**: For cloning the repository

### Quick Start

```bash
# Clone the repository
git clone https://github.com/dnxbatista/penguinpad
cd penguinpad

# Create build directory
mkdir build && cd build

# Configure and build
cmake ..
cmake --build . --config Release

# Run (Windows)
./Release/penguinpad.exe

# Run (Linux/macOS)
./penguinpad
```

### Build Options

```bash
# Debug build
cmake .. -DCMAKE_BUILD_TYPE=Debug
cmake --build .

# Release with optimizations
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build .

# Install to system (optional)
sudo cmake --install .
```

## Usage

1. Connect your gamepad to your computer
2. Launch PenguinPad
3. The interface will automatically detect your controller
4. Press buttons and move sticks to see real-time feedback
5. Press **F8** to toggle the ImGui demo window (for developers)

### Supported Controllers

PenguinPad supports any controller recognized by SDL3, including:
- Xbox Controllers (Series X/S, One, 360)
- PlayStation Controllers (DualSense, DualShock 4)
- Nintendo Switch Pro Controller
- Generic USB/Bluetooth gamepads

## Building from Source

### Windows (Visual Studio)

```bash
cmake -B build -G "Visual Studio 17 2022"
cmake --build build --config Release
```

### Linux

```bash
# Install dependencies (Ubuntu/Debian)
sudo apt-get install build-essential cmake git

# Build
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make -j$(nproc)
```

### macOS

```bash
# Install dependencies
brew install cmake

# Build
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make -j$(sysctl -n hw.ncpu)
```

## Contributing

Contributions are welcome! Please see [CONTRIBUTING.md](CONTRIBUTING.md) for details.

1. Fork the Project
2. Create your Feature Branch (`git checkout -b feature/AmazingFeature`)
3. Commit your changes (`git commit -m 'Add: Amazing new feature'`)
4. Push to the Branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request

## License

Distributed under the MIT License. See [LICENSE](LICENSE) for more information.

## Acknowledgments

- [SDL3](https://github.com/libsdl-org/SDL) - Simple DirectMedia Layer
- [Dear ImGui](https://github.com/ocornut/imgui) - Immediate Mode GUI
- [Inter Font](https://rsms.me/inter/) - UI Typography

## Contact

Project Link: [https://github.com/dnxbatista/penguinpad](https://github.com/dnxbatista/penguinpad)

---

**Note**: If you encounter any issues, please check the [Issues](https://github.com/dnxbatista/penguinpad/issues) page or create a new one.
