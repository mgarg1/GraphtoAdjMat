#Graph to Adjacency Matrix
-----------------------------

##Input Images

###Restrictions on the image

1. Radius of the circles should be between `0` to `50` pixels 
2. Two vertices cannot be in one line
3. All the node labels should be from the templates provided
4. Minimum length of the line should be `minLineAccuTHRES = 80` pixels

###Future Additions

1. 




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