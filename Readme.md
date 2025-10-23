# Visualization of Berzier Curves in Raylib
- Quadratic berzier curves:
    Curves made from 3 points, (a, b, c).
    Using the "lerp" function to calculate the points that make up the curve.
    Lerp is defined as follows: Lerp(a,b,t) = (b - a)*t + a;
    Where a and b are the two points and t is a kinda offset from 0 to n.
    We get the point on the line after three operations:
    $$
    \begin{align*}
    (1) \quad & p_1 = \text{Lerp}(a, b, t) = (b - a)t + a \\[0.5em]
    (2) \quad & p_2 = \text{Lerp}(b, c, t) = (c - b)t + b \\[0.5em]
    (3) \quad & p_{\text{final}} = \text{Lerp}(p_1, p_2, t)
    \end{align*}
    $$
    Furthermore, to get more of them we change t from 0 to n, where n is
    the number of points we want between a and c.
