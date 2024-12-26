This Folder contains the Visual Studio Project of **Bressenham's Algorithm**

# Bressenham's Algorithm

Bresenham's Line Algorithm is an efficient algorithm used in computer graphics to draw a straight line between two points on a raster grid.
It is highly efficient because it uses only integer arithmetic and eliminates the need for floating-point calculations, making it faster than other line-drawing algorithms like DDA.

### Key Idea
The algorithm increments one pixel at a time, calculating which pixel (either straight or diagonal) is closest to the theoretical line at each step, using a decision parameter.

## Steps of the Bressenham's Algorithm

1) Input the Line Endpoints

2) Calculate the Differences :- dy & dx

3) Initialize the Decision Parameter :- P = 2dy - dx

4) Iterate Over X :- 

If **p<0**, the next pixel is horizontally adjacent. Update p as : p = p + 2dy
If **p>0**, the next pixel is horizontally adjacent. Update p as : p = p + 2dy - 2dx

5) Plot the Points

## Advantages
- Uses only integer calculations, making it faster than floating-point-based algorithms.
- Produces accurate results for all slopes.
- Efficient for real-time rendering.

## Disadvantages
- Limited to straight lines.
- For curved lines or other shapes, different algorithms are needed.
