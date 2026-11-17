# 3D Rubik's Cube Simulator (Computer Graphics Project)

An interactive, realistic 3D Rubik's Cube simulator built using **C++**, **OpenGL (Fixed-Function Pipeline)**, and **FreeGLUT**.

Designed for **B.Tech Computer Science & Engineering** Computer Graphics coursework, lab demonstrations, and viva examinations.

---

## 1. Features

- **3×3×3 Cube Structure**: Composed of 27 individually modeled, textured, and rotated cubies.
- **Accurate Colors**: Standard competition color layout (White, Yellow, Green, Blue, Red, Orange) with matte dark charcoal internal faces.
- **Smooth Slice Animations**: 60 FPS interpolated face and middle-slice rotations ($5^\circ$ to $6^\circ$ per frame).
- **Interactive 3D Camera**: Full 360-degree spherical orbit (pitch/yaw) and smooth zoom.
- **Realistic Lighting**: Fixed-function Phong ambient and diffuse illumination using dual directional light sources with surface normal vectors.
- **Dark Neutral Background**: Full-screen vertical gradient background for contrast.
- **Scramble & Reset**: Single-key random scrambler (SPACE or S) and instant reset to solved state (0 or C).
- **On-Screen HUD Overlay**: Real-time Move counter, last move indicator, and on-screen control cheat-sheet matching reference display.
- **Wireframe Mode**: Instant toggle (`W`) between filled 3D shaded polygon mode and wireframe structure.
- **Modular Object-Oriented Architecture**: Clean separation between `Cubie`, `RubiksCube`, `Camera`, `Renderer`, `InputHandler`, and `main.cpp`.

---

## 2. Project Structure

```text
RubiksCubeSimulator/
├── .vscode/
│   ├── tasks.json             # VS Code build task (Ctrl+Shift+B)
│   ├── launch.json            # VS Code run/debug task (F5)
│   └── c_cpp_properties.json  # VS Code IntelliSense & include paths
├── include/
│   └── GL/
│       ├── freeglut.h         # FreeGLUT primary header
│       ├── freeglut_ext.h
│       ├── freeglut_std.h
│       └── glut.h             # Standard GLUT wrapper header
├── lib/
│   ├── libfreeglut.a          # 32-bit MinGW link library
│   └── x64/
│       ├── libfreeglut.a      # 64-bit MinGW link library
│       └── freeglut.dll       # 64-bit runtime DLL
├── src/
│   ├── Common.h               # Global constants, colors, math helpers
│   ├── Cubie.h / .cpp         # Individual cubelet geometry & local matrices
│   ├── RubiksCube.h / .cpp    # 27-cubie scene graph, slice turns & animation
│   ├── Camera.h / .cpp        # 3D view orbit and zoom transformations
│   ├── Renderer.h / .cpp      # OpenGL lighting, background & rendering
│   ├── InputHandler.h / .cpp  # Keyboard and mouse dispatching
│   └── main.cpp               # Entry point and GLUT callbacks
├── build.bat                  # One-click Windows build script
├── run.bat                    # One-click Windows run script
├── freeglut.dll               # Pre-placed runtime DLL for executable
├── RubiksCubeSimulator.cbp    # Code::Blocks Project file (for college labs)
├── VIVA_GUIDE.md              # Complete Computer Graphics viva preparation guide
└── README.md                  # This file
```

---

## 3. Controls

### Mouse
| Action | Function |
| :--- | :--- |
| **Left Click + Drag** | Orbit / Rotate the entire 3D cube view |
| **Right Click + Drag** | Zoom camera in / out |
| **Mouse Scroll Wheel** | Smooth zoom in / out |

### Keyboard Face Turns
| Key | Action | Notation |
| :--- | :--- | :--- |
| `R` / `r` | Right Face | Clockwise (`R`) / Counter-Clockwise (`R'`) |
| `L` / `l` | Left Face | Clockwise (`L`) / Counter-Clockwise (`L'`) |
| `U` / `u` | Up / Top Face | Clockwise (`U`) / Counter-Clockwise (`U'`) |
| `D` / `d` | Down / Bottom Face | Clockwise (`D`) / Counter-Clockwise (`D'`) |
| `F` / `f` | Front Face | Clockwise (`F`) / Counter-Clockwise (`F'`) |
| `B` / `b` | Back Face | Clockwise (`B`) / Counter-Clockwise (`B'`) |

### Keyboard Middle Slice Turns
| Key | Action |
| :--- | :--- |
| `V` / `v` | Vertical Middle Slice ($X = 0$) |
| `H` / `h` | Horizontal Middle Slice ($Y = 0$) |
| `M` / `m` | Front-Back Middle Slice ($Z = 0$) |

### Utility & Camera Shortcuts
| Key | Action |
| :--- | :--- |
| `SPACE` / `S` | **Scramble** the cube with 15 random face turns |
| `0` / `C` | **Reset** cube to initial solved state |
| `W` / `w` | **Toggle Wireframe** mode (`GL_LINE` vs `GL_FILL`) |
| `+` / `-` | Zoom camera in / out |
| **Arrow Keys** | Rotate view angle via keyboard |
| `9` / `Home` | Reset camera to default perspective |
| `ESC` | Exit application |

---

## 4. How to Open and Run in VS Code

1. Open **Visual Studio Code**.
2. Click **File -> Open Folder...** and select the `RubiksCubeSimulator` folder.
3. Open the integrated terminal (`Ctrl + ~`) or press:
   - **`Ctrl + Shift + B`** to trigger the build task (`build.bat`).
   - Run `.\run.bat` or `.\RubiksCube.exe` in the terminal to launch the simulator.

---

## 5. How to Compile from Command Prompt / Terminal

Open Command Prompt (`cmd.exe`) or PowerShell in the `RubiksCubeSimulator` directory and run:

```cmd
build.bat
run.bat
```

Or compile manually using `g++`:

```cmd
g++ -O2 -Wall src/main.cpp src/Cubie.cpp src/RubiksCube.cpp src/Camera.cpp src/Renderer.cpp src/InputHandler.cpp -Iinclude -Llib/x64 -Llib -lfreeglut -lopengl32 -lglu32 -lwinmm -lgdi32 -o RubiksCube.exe
```

---

## 6. How to Open in Code::Blocks (for College Lab PCs)

If your college laboratory uses **Code::Blocks**:
1. Open Code::Blocks.
2. Click **File -> Open...** and select `RubiksCubeSimulator.cbp`.
3. Press **F9** (or click the green **Build and Run** gear/play button).

---

## 7. Troubleshooting Common Issues

- **`freeglut.dll was not found`**:
  Make sure `freeglut.dll` is in the same directory as `RubiksCube.exe`. Both 32-bit and 64-bit binaries are included in the `lib/` folder.
- **`undefined reference to glutInit / glBegin`**:
  Ensure the linker flags `-lfreeglut -lopengl32 -lglu32 -lwinmm -lgdi32` are provided in that exact order.
