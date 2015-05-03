#Graph to Adjacency Matrix
-----------------------------

##Method1

###Alogrithm

1. Applied Hough Transform to find Circles
2. Filter Circles by comparing **EXPLAIN**
3. Applied Hough Transform to find lines 
4. Filter Lines by comparing **EXPLAIN**
5. Name each vertex by doing the template matching in the region near the center for each circle
6. For every Pair of circle check if there exists a line in the lineVector in the close proximity of the line joining the centers of the two circles 

###Code Notes




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