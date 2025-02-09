This Folder contains the Visual Studio Project of **Line Clipping in Computer Graphics Made with the help of OpenGL & C++**

# **Line Clipping in Computer Graphics**
Line clipping is a fundamental technique in computer graphics used to determine which portions of a line segment are within a defined clipping region.
This ensures that only the visible parts of a line are rendered, improving efficiency and visual correctness in graphical applications.

## Clipping Window

A clipping window defines the area where lines should be visible. Any part of a line outside this window is clipped (removed).

## Line Clipping Algorithms

1. **Cohen-Sutherland Line Clipping Algorithm**

This algorithm efficiently clips a line against a rectangular window using outcodes (binary region codes) to classify endpoints.

### Steps:

- Compute the outcodes for both endpoints.
  
- Apply the bitwise AND operation:
  If (outcode1 & outcode2) ≠ 0, the line is completely outside (reject).
  If (outcode1 | outcode2) = 0, the line is completely inside (accept).

- If the line is partially inside, compute intersection points with the window edges and recursively check the new segment.

2. **Liang-Barsky Line Clipping Algorithm**

A more efficient algorithm than Cohen-Sutherland, using parametric equations to clip lines against a rectangular window.

### Steps:

- Express the line segment parametrically as P(t) = P1 + t(P2 - P1).

- Compute the entering and leaving intersection points using inequalities.

- Clip the line efficiently using calculated t values.


## **Applications**

- Computer Graphics Rendering (e.g., CAD software, game engines)
- Geographical Mapping (e.g., GIS systems)
- Graphical User Interfaces (e.g., windowing systems)
