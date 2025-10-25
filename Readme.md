# Visualization of Berzier Curves in Raylib
- Quadratic berzier curves:<br>
    A Bézier curvex is a parametric curve used in computer graphics and related fields.<br>
    A set of discrete "control points" defines a smooth, continuous curve by means of a formula. <br>
    Let $p_1(x,y)$, $p_2(x,y)$, $p_3(x,y)$ be our control points, then the formula defining points on
    the curve is as follows:<br>
    - $l_1 = f(p_1, p_2, t) = (p_2 - p_1)*t + p_1$
    - $l_2 = f(p_2, p_3, t) = (p_3 - p_2)*t + p_2$
    - $P_t = f(l_1,l_2,t)$ 

    $P_t$ is then one of points on the curve. We change $t$ according to
    how many points we want. <br> Since t must be between $0$ and $1$, we calculate it like this:<br>
    $t = \frac i n$ where $i$ is the index of the selected point we want to display.    
