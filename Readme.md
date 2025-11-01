# Visualization of Berzier Curves in Raylib
- ### Quadratic berzier curves:<br>
    A Bézier curvex is a parametric curve used in computer graphics and related fields.<br>
    A set of discrete "control points" defines a smooth, continuous curve by means of a formula. <br>
    Let $p_1(x,y)$, $p_2(x,y)$, $p_3(x,y)$ be our control points, then the formula defining points on
    the curve is as follows:<br>
    - $l_1 = f(p_1, p_2, t) = (p_2 - p_1)t + p_1$
    - $l_2 = f(p_2, p_3, t) = (p_3 - p_2)t + p_2$
    - $P_t = f(l_1,l_2,t)$ 

    $P_t$ is then one of points on the curve. We change $t$ according to
    how many points we want. <br> Since $t$ must be between $0$ and $1$, we calculate it like this:<br>
    $t = \frac i n$ where $i$ is the index of the selected point we want to display.    

- ### Cubic berzier curve:<br>
    The formula for these is similar as for the quadratic ones.<br>
    We now have $4$ control points, where $3$ of them each make up one part of the final curve,<br>
    while sharing one point, $t$ is calculated the same way.<br>
    If we reuse the same points from above and add $p_4(x,y)$ then:<br>
    - $l_1$ and $l_2$ are the same as above, and so is the $P_{t_1}$.
    - We get $P_{t_2}$ by using the formula on $p_2$, $p_3$, and $p_4$.<br>
    - Finally, we get the $P_t$ by using the formula again on $P_{t_1}$ and $P_{t_2}$. <br>

- ### Quadratic and Cubic Splines:<br>
    To display enclosed areas between the berzier curves aka 'splines', we are calculate the intersection between <br>
    the curve and the row for each column of the grid.<br>
    We can get $x$ and $y$ coordinates of the point on the curve using the same equations that are hidden inside<br>
    the lerp functions above, just by modifing them a little :<br>

    - $\{x, y\}$ $\rightarrow$ $t = (p_{23} - p_{12})t^2 + 2p_{12}t + p_1$<br>
    - $dt = 2(p_{23} - p_{12})t + p_{12}$

    Where $p_{12} = p2-p1$ and so on. The first function gives us the points on the curve based on $t$<br> and the second one is the
    derivative of one of the functions so we can find which way the curve is heading, since<br>
    the derivaitve is positive, if the function is ascending and negative the other way.<br>
    We used the same tehique for cubic ones too, but they are way more complicated<br> since we get
    a polinominal by expanding the lerp functions:<br>
    - $\{x, y\}$ $\rightarrow$ $t = (p_{12} + 2p_{23} + p_{34})t^3 + (-3p_{12} + 3p_{23})t^2 + 3p_{12}t + p_1$<br>
    - $dt = 3(p_{12}(t - 1)^2 + t(p_{34}t - 2p_{23}(t - 1)))$<br>
    
    Note:<br>
    The math and code for quadratic berzier curves was inspired by this youtube video: [click](https://www.youtube.com/watch?v=1epwf3iaQNU),
    while the cubic berzier curves i have implemented by myself.<br> 
    Both feature some bugs when rendering splines, make sure to place the control points in $\bold{ clockwise \space pattern!}$<br>
