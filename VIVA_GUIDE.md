# Computer Graphics Project & Viva Presentation Guide
## 3D Rubik's Cube Simulator in C++ and OpenGL

This guide contains the theoretical concepts, mathematical formulas, and frequently asked viva questions to help you confidently present this project to your Computer Graphics professor.

---

## 1. Project High-Level Architecture

The project models a classical **3x3x3 Rubik's Cube** utilizing a hierarchical scene graph:
- **Base Level (`Cubie`)**: 27 individual cubelets modeled with local coordinates in $[-s, s]^3$. Each cubie maintains its own $4 \times 4$ accumulated orientation matrix.
- **Mid Level (`RubiksCube`)**: Orchestrates the 27 cubelets, handles slice selection (X, Y, Z axes at $-1, 0, 1$), computes continuous interpolation for animations, and performs discrete coordinate permutations upon move completion.
- **View Level (`Camera`)**: Implements an orbiting perspective camera governed by spherical Euler angles (elevation/pitch and azimuth/yaw) and uniform scaling (zoom).
- **Presentation Level (`Renderer`)**: Configures dual-light Phong illumination, depth buffering (Z-buffer), and a full-screen background gradient pass.
- **Control Level (`InputHandler`)**: Decouples FreeGLUT OS event callbacks from application state.

---

## 2. Core Computer Graphics Concepts Demonstrated

### A. The 3D Transformation Pipeline

Every vertex $V_{\text{local}}$ of a cubie passes through four coordinate spaces before reaching the monitor screen:

$$\mathbf{V}_{\text{screen}} = \mathbf{M}_{\text{viewport}} \times \mathbf{M}_{\text{projection}} \times \mathbf{M}_{\text{view}} \times \mathbf{M}_{\text{model}} \times \mathbf{V}_{\text{local}}$$

1. **Modeling Transformation ($\mathbf{M}_{\text{model}}$)**:
   - Moves the cubie from its local origin to its grid position: `glTranslatef(x * SPACING, y * SPACING, z * SPACING)`.
   - Multiplies its accumulated rotation matrix: `glMultMatrixf(transform)`.
   - If currently animating, rotates the entire slice about the origin: `glRotatef(animAngle * animDir, axis)`.

2. **Viewing Transformation ($\mathbf{M}_{\text{view}}$)**:
   - Positions the camera at distance $d$: `glTranslatef(0.0f, 0.0f, -distance)`.
   - Scales the view for zoom: `glScalef(zoom, zoom, zoom)`.
   - Orbits around the cube center: `glRotatef(xRot, 1, 0, 0)` and `glRotatef(yRot, 0, 1, 0)`.

3. **Projection Transformation ($\mathbf{M}_{\text{projection}}$)**:
   - Sets up a 3D perspective frustum: `gluPerspective(fovY, aspect, near, far)`.
   - Distant objects appear smaller than nearby objects, creating realistic depth perception.

4. **Viewport Transformation ($\mathbf{M}_{\text{viewport}}$)**:
   - Maps normalized device coordinates $[-1, 1]^3$ onto window pixel coordinates $[0, W] \times [0, H]$ via `glViewport(0, 0, width, height)`.

---

### B. Matrix Stack Management (`glPushMatrix` and `glPopMatrix`)

- **Why is it needed?**
  Transformations in OpenGL are cumulative. Without isolating them, rotating one cubie would unintentionally rotate all subsequent cubies rendered after it.
- **How it works:**
  - `glPushMatrix()` saves the current Modelview matrix onto the GPU matrix stack.
  - Local translations and rotations are applied, and the cubie is drawn.
  - `glPopMatrix()` restores the previous matrix, leaving the graphics pipeline clean for the next object.

---

### C. Rotation Mathematics and Zero-Drift Coordinate Updates

When an animated $90^\circ$ face turn finishes:
1. **Matrix Accumulation**:
   $$\mathbf{M}_{\text{new}} = \mathbf{R}_{90^\circ} \times \mathbf{M}_{\text{old}}$$
   Computed via `glMultMatrixf` so that individual cubie orientations persist across future turns.

2. **Discrete Integer Trigonometric Rotation**:
   Instead of updating $(x, y, z)$ through floating-point matrix multiplication (which accumulates numerical round-off error over time), the logical grid coordinates are rotated using exact integer trigonometry for $\theta = \pm 90^\circ$:

   - **Rotation about X-axis**:
     $$\begin{cases} x' = x \\ y' = y\cos\theta - z\sin\theta \\ z' = y\sin\theta + z\cos\theta \end{cases}$$
   - **Rotation about Y-axis**:
     $$\begin{cases} x' = x\cos\theta + z\sin\theta \\ y' = y \\ z' = -x\sin\theta + z\cos\theta \end{cases}$$
   - **Rotation about Z-axis**:
     $$\begin{cases} x' = x\cos\theta - y\sin\theta \\ y' = x\sin\theta + y\cos\theta \\ z' = z \end{cases}$$

   Because $\cos(\pm 90^\circ) = 0$ and $\sin(\pm 90^\circ) = \pm 1$, $(x', y', z')$ remain strictly integers in $\{-1, 0, 1\}$, **guaranteeing zero drift**.

---

### D. Lighting, Shading, and Surface Normals

- **Illumination Model**: OpenGL Fixed-Function Phong Model.
  $$I = I_{\text{ambient}} + I_{\text{diffuse}} = k_a I_a + k_d I_d (\mathbf{N} \cdot \mathbf{L})$$
- **Surface Normals (`glNormal3f`)**:
  - Each face quad specifies an outward unit normal vector $\mathbf{N}$ perpendicular to its plane:
    - Right: $(1, 0, 0)$
    - Left: $(-1, 0, 0)$
    - Top: $(0, 1, 0)$
    - Bottom: $(0, -1, 0)$
    - Front: $(0, 0, 1)$
    - Back: $(0, 0, -1)$
- **Dual Light Configuration**:
  - `GL_LIGHT0` (Key Light): Placed at $(6, 8, 7)$ to illuminate the front, top, and right faces.
  - `GL_LIGHT1` (Fill Light): Placed at $(-6, -6, -6)$ with lower intensity to prevent back faces from becoming completely dark.

---

### E. Hidden Surface Removal (Z-Buffering)

- Enabled with `glEnable(GL_DEPTH_TEST)` and cleared each frame with `glClear(GL_DEPTH_BUFFER_BIT)`.
- For each rasterized fragment, the GPU checks its depth $Z$ against the depth buffer. If the fragment is closer to the camera than previously rendered fragments at that pixel, its color is written and its depth updated; otherwise, it is discarded.

---

### F. Double Buffering (`GLUT_DOUBLE`)

- In single buffering, drawing directly to the screen produces visible tearing and flickering as polygons are cleared and redrawn.
- Double buffering uses two memory buffers:
  - **Back Buffer**: The scene is drawn off-screen.
  - **Front Buffer**: Displayed on the monitor.
  - `glutSwapBuffers()` swaps the two buffers instantaneously during the vertical blanking interval, producing smooth 60 FPS animation.

---

## 3. Top 10 Viva Questions & Model Answers

### Q1: What is the difference between Modelview and Projection transformations?
> **Answer**: The **Modelview transformation** positions and orients objects in the 3D world and defines where the camera is located relative to those objects (World/Eye space). The **Projection transformation** defines the viewing volume (frustum) and specifies how 3D coordinates are projected onto a 2D plane (Perspective or Orthographic).

### Q2: Why did you use `gluPerspective` instead of `glOrtho`?
> **Answer**: `gluPerspective` creates perspective projection with a vanishing point, where objects farther from the camera appear smaller, mimicking human vision. `glOrtho` produces parallel projection with no depth distortion, which would look unnatural for a 3D Rubik's cube.

### Q3: How do you rotate a single slice without rotating the rest of the cube?
> **Answer**: Each cubie has discrete coordinates $(x, y, z) \in \{-1, 0, 1\}$. When rotating a slice (e.g., the Right face where $x = 1$), only cubies satisfying $c.x == 1$ receive the dynamic rotation `glRotatef(angle, 1, 0, 0)`. The remaining 18 cubies are drawn without this extra rotation.

### Q4: How is gimbal lock avoided in this project?
> **Answer**: Gimbal lock occurs when three sequential Euler angle rotations align two rotational axes, losing a degree of freedom. In our project, slice turns are restricted to single-axis $90^\circ$ increments, and the resulting rotation is stored directly as a $4 \times 4$ transformation matrix (`float transform[16]`) rather than accumulating continuous Euler angles.

### Q5: What role do surface normals play in OpenGL rendering?
> **Answer**: Surface normals are unit vectors perpendicular to the polygon surface. OpenGL uses the dot product between the surface normal $\mathbf{N}$ and the light direction vector $\mathbf{L}$ ($\mathbf{N} \cdot \mathbf{L}$) to calculate Lambertian diffuse reflection, giving faces their 3D shading.

### Q6: How does the camera orbit around the cube?
> **Answer**: When the user drags the mouse, the displacement $\Delta x$ and $\Delta y$ adjusts two Euler angles: `yRot` (azimuth) and `xRot` (elevation). In the Modelview matrix, the world is translated back by `-distance`, then rotated by `xRot` around the X-axis and `yRot` around the Y-axis, creating the visual effect of orbiting around the cube center $(0,0,0)$.

### Q7: Why do we disable depth testing when drawing the background gradient?
> **Answer**: The background is a 2D quad drawn across the full screen $[-1, 1]$. By disabling `GL_DEPTH_TEST` and `GL_LIGHTING` during this pass, the background quad does not write into or check the depth buffer, ensuring all subsequent 3D cubies are rendered on top of it.

### Q8: What is the purpose of `glutTimerFunc`?
> **Answer**: `glutTimerFunc(16, callback, 0)` schedules a callback function to run after 16 milliseconds (~60 frames per second). Inside the callback, animation angles are updated, `glutPostRedisplay()` triggers a redraw, and the timer is re-registered for smooth frame-rate-governed animation.

### Q9: How are internal faces hidden or distinguished from exterior faces?
> **Answer**: During initialization, cubies check their coordinates. If a face points inward (e.g., a cubie with $x = 0$ has no exterior X-faces), that face is assigned `COLOR_INNER` (matte dark black). Exterior faces are assigned bright Rubik's sticker colors.

### Q10: How are edge outlines drawn on the cubelets?
> **Answer**: Outlines are drawn using `GL_LINES` with `glLineWidth(2.5f)` connecting the 12 edges of each cubie in dark charcoal with lighting temporarily disabled. This creates a distinct beveled appearance between adjacent stickers.

### Q11: How is the 2D HUD (on-screen text overlay) rendered over the 3D perspective scene?
> **Answer**: By switching projection modes within the same frame. First, the 3D cube is rendered using `gluPerspective`. Then `renderHUD()` temporarily disables lighting and depth testing (`glDepthMask(GL_FALSE)`), pushes the projection and modelview matrices, sets up a 2D screen-pixel coordinate system using `gluOrtho2D(0, width, 0, height)`, positions and draws bitmap text via `glRasterPos2f` and `glutBitmapCharacter`, and finally pops both matrix stacks to restore the 3D pipeline cleanly.
