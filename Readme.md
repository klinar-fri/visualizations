# Visualization of Berzier Curves in Raylib
- Quadratic berzier curves:
    Curves made from 3 points, (a, b, c).
    Using the "lerp" function to calculate the points that make up the curve.
    Lerp is defined as follows: Lerp(a,b,t) = (b - a)*t + a;
    Where a and b are the two points and t is a kinda offset from 0 to n.
    We get the point on the line after three operations:
    1. p1 = Lerp(a,b,t) = (b - a)*t + a;
    2. p2 = Lerp(b,c,t) = (c - b)*t + b;
    3. point = Lerp(p1, p2, t);
    Furthermore, to get more of them we change t from 0 to n, where n is
    the number of points we want between a and c.
