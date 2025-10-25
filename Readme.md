# Visualization of Berzier Curves in Raylib
- Quadratic berzier curves:
    A Bézier curvex is a parametric curve used in computer graphics and related fields.<br>
    A set of discrete "control points" defines a smooth, continuous curve by means of a formula. <br>
    Let $p_1(x,y)$, $p_2(x,y)$, $p_3(x,y)$ be our control points, then the formula defining points on
    the curve is as follows:<br>
    - $L_1 = f(p_1, p_2, t) = (p_2 - p_1)*t + p_1$
    - $L_2 = f(p_2, p_3, t) = (p_3 - p_2)*t + p_2$
    - $P_t = f(L_1,L_2,t)$
