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

- Qubic berzier curve:<br>
    The formula for these is similar as for the quadratic ones.<br>
    We now have $4$ control points, where $3$ of them each make up one part of the final curve,
    while sharing one point, $t$ is calculated the same way.<br>
    If we reuse the same points from above and add $p_4(x,y)$ then:<br>
    - $l_1$ and $l_2$ are the same as above, and so is the $P_{t_1}$.
    - We get $P_{t_2}$ by using the formula on $p_2$, $p_3$, and $p_4$.<br>
    - Finally, we get the $P_t$ by using the formula again on $P_{t_1}$ and $P_{t_2}$. <br>