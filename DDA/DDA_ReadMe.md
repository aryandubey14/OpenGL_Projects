This Folder contains the Visual Studio Project of **Digital Differential Analyzer Algorithm (DDA)**

# DDA

The Digital Differential Analyzer (DDA) algorithm is a popular method used in computer graphics to draw lines between two points on a grid or screen.
It is an incremental method that computes the intermediate points needed to render the line by calculating the differences in the x and y coordinates.

## Steps of the DDA Algorithm

1) Input the Line Endpoints
2) Calculate the Differences
3) Determine the Number of Steps
4) Calculate the Increment Values
5) Iteratively Compute Points
6) Plot the Points

## Advantages
- Simple to implement.
- Works for all line slopes (positive, negative, steep, shallow).

## Disadvantages
- Requires floating-point arithmetic, which may slow down execution.
- Accumulated round-off errors can affect accuracy.
