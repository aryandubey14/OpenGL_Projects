This Folder contains the Visual Studio Project of **Bézier curves in Computer Graphics**

# Bézier curves

Bézier curves are parametric curves frequently used in computer graphics for modeling smooth and scalable shapes. They are widely employed in vector graphics, animation, font design, and CAD applications.

## Types of Bézier Curves

Bézier curves can be classified based on the number of control points:

- **Linear Bézier Curve (2 control points** - a straight line)
- **Quadratic Bézier Curve (3 control points** - a parabolic shape)
- **Cubic Bézier Curve (4 control points** - the most commonly used type in graphics)
- **Higher-Order Bézier Curves** (more than 4 control points - offer finer control but are computationally expensive)

## Mathematical Representation

A Bézier curve of degree  is defined as:

### **B(t)= ∑ BiPi** [0<=t<=1]

where, i = 0 -> n
- Pi are the control points
- Bi are the Bernstein polynomials


## Applications

- Computer Graphics & Animation (Vector graphics, motion paths)
- Font Rendering (TrueType and PostScript fonts)
- CAD & 3D Modeling (Smooth curve design in engineering)
- Image Processing (Edge smoothing, curve fitting)
