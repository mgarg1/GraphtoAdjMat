# Graph to Adjacency Matrix
-----------------------------

## Description
This project is about converting an image of Graph into its corresponding adjacency Matrix. This task can be achieved in various ways and each method is maintained in different directory. This project uses [EasyBMP](http://easybmp.sourceforge.net/) as the C++ bitmap image processing library.

### Input:
![A unweighted undirected Graph](https://raw.githubusercontent.com/mgarg1/GraphtoAdjMat/master/InputImage/ExampleInput.png)

### Output:
```
        A       C       B       D
A       -       1       1       0
C       1       -       1       1
B       1       1       -       1
D       0       1       1       -
```
### Output Image:
![Marked lines and circles](https://raw.githubusercontent.com/mgarg1/GraphtoAdjMat/master/InputImage/ExampleOutput.png)

## Usage
* To compile the most relevant implementation just use make
> make
* To compile the implementation of particular method use the method name as argument to make
> make method1.out
* To execute: 
>./method1.out 0 &lt;Input File&gt; [Output File]

## Project Structure
Folder | Description
----------|-------------------
*EasyBMP* | Image processing Library Source
*DB* | this folder contains the template images for template Matching
*InputImage* | Images for test Input
*myUtilities* | Some small ~functions~ utilities for code reuse
*templateMatch* | Library for Character recognition does simple template Matching
*Method[1-N]* | Different Implementations for this project
*nouse* | waste code

## References
Exact Algorithms used is defined in the respective method folder this section tells about the general DIP algorithms used in this project
- [Hough Transform for Line](http://en.wikipedia.org/wiki/Hough_transform#Implementation)
- [Hough Transform for Circle](http://en.wikipedia.org/wiki/Hough_transform#Circle_detection_process)
- [Flood Fill Algorithm](http://en.wikipedia.org/wiki/Flood_fill)
- [Template Matching for Character Recognition](http://en.wikipedia.org/wiki/Template_matching)
- [Brehensen's line Algorithm](http://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm)
- [Otsu's method](http://en.wikipedia.org/wiki/Otsu%27s_method) for automatic image thresholding

## Things yet to implement/Change
- [X] working on threshold
- [] limit line segments
- [] weighted graph
- [] directed graph

<!--
Method1:
Uses Approximation of the line between circles

Method2:
Color the line found out by Hough transform and Find the max and 
Min point as end of the line segment
Color only if there exists a white pixel beneath the line
=======
Color the line found out by Hough tranform and Find the max and 
min point as end of the line segment
COlor only if there exists a white pixel beneath the line

Use flood fill to

Method3:
Apply Hough transform between Circles

Method4:
-->
