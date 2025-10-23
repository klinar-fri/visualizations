# Visualization of Berzier Curves in Raylib
- Quadratic berzier curves:
    Curves made from 3 points, (a, b, c). <br>
    Using the "lerp" function to calculate the points that make up the curve.<br>
    Lerp is defined as follows: Lerp(a,b,t) = (b - a)*t + a; <br>
    Where a and b are the two points and t is a kinda offset from 0 to n. <br>
    We get the point on the line after three operations:
    - `p1 = Lerp(a,b,t) = (b - a)*t + a;`
    - `p2 = Lerp(b,c,t) = (c - b)*t + b;`
    - `point = Lerp(p1, p2, t);` <br>
    Furthermore, to get more of them we change t from 0 to n, where n is
    the number of points we want between a and c.
