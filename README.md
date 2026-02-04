# PenguinPad
A lightweight and efficient tool to test gamepad inputs, developed in C++

## Features
- Real-time Detection: Instantly identifies when a controller is connected or disconnected.
- Visual Mapping: Visualize buttons, analog sticks (axes), and triggers through an intuitive UI.
- SDL3 Support: Leverages the latest hardware management improvements from SDL3.
- Performance: Minimal resource footprint thanks to ImGui's immediate mode rendering.

## Getting Started
### Prerequisites
Ensure you have the following installed on your system:
1. A C++ compiler (GCC, Clang, or MSVC).
2. CMake (version 3.10 or higher).
   
### Compilation
1. Clone the repository:

``git clone https://github.com/dnxbatista/penguinpad``

``cd penguinpad``

2. Make a build dir

``mkdir build``

``cd build``

3. Configure and build

``cmake ..``

``cmake --build .``

4. Run the .exe file in

``penguinpad/build/Debug/penguinpad.exe``

---
## Contributing
If you want <3

1. Fork the Project.
2. Create your Feature Branch (git checkout -b feature/AmazingFeature).
3. Commit your changes (git commit -m 'Add some AmazingFeature').
4. Push to the Branch (git push origin feature/AmazingFeature).
5. Open a Pull Request.
---
## License
Distributed under the MIT License.
