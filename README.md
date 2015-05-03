#Graph to Adjacency Matrix
-----------------------------

##Description
This project is about converting an image of Graph into its corresponding adjacency Matrix. This task can be achieved in various ways and each method is maintained in different directory. This project uses [EasyBMP](http://easybmp.sourceforge.net/) as the C++ bitmap image processing library.

##Usage
* To compile the current working implementation 
	* use the make file in current directory
* To compile the implementation of particular method use their respective makefiles
* To execute: 
```
	./prog1.out 0 [Input File] 
```

##Project Structure
Folder | Description
----------|-------------------
*EasyBMP* | Image processing Library Source
*DB* | this folder contains the template images for template Matching
*InputImage* | Images for test Input
*myUtilities* | Some small ~functions~ utilities for code reuse
*templateMatch* | Library for Character recognition does simple template Matching
*Method[1-N]* | Different Implementations for this project
*nouse* | waste code

##References
Exact Algorithms used is defined in the respective method folder this section tells about the general DIP algorithms used in this project
- [Hough Transform for Line](http://en.wikipedia.org/wiki/Hough_transform#Implementation)
- [Hough Transform for Circle](http://en.wikipedia.org/wiki/Hough_transform#Circle_detection_process)
- [Flood Fill Algorithm](http://en.wikipedia.org/wiki/Flood_fill)
- [Template Matching for Character Recognition](http://en.wikipedia.org/wiki/Template_matching)
- [Brehensen's line Algorithm](http://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm)
- [Otsu's method](http://en.wikipedia.org/wiki/Otsu%27s_method) for automatic image thresholding

##Things yet to implement/Change
- [] working on threshold
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

Use flood fill to

Method3:
Apply Hough transform between Circles

Method4:
-->
