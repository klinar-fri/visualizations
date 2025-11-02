#include "raylib.h"
#include "raymath.h"
#include <math.h>
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

vector<Vector2> points;
int pointDragging = -1;

// aspect ratio 4:3
constexpr int heightFactor = 3;
constexpr int widthFactor = 4;
constexpr int windowSize = 200;

constexpr int windowHeight = heightFactor * windowSize;
constexpr int windowWidth = widthFactor * windowSize;

// grid size
constexpr int gridFactor = 20;
constexpr int gridWidth = gridFactor * widthFactor;
constexpr int gridHeight = gridFactor * heightFactor;

constexpr int cellWidth = windowWidth / gridWidth;
constexpr int cellHeight = windowHeight / gridHeight;

bool grid[gridHeight][gridWidth] = {0};

typedef struct {
   float tx;
   float d; 
} Solution;

vector<Solution> solutions;

void displayPoints(){
    Vector2 mouse = GetMousePosition();
    int newPointToDrag = -1;

    for(size_t i = 0; i < points.size(); i++){
        Vector2 size = {20, 20};
        Vector2 position = points[i];
        position = Vector2Subtract(position, Vector2Scale(size, 0.5));
        bool hover = CheckCollisionPointRec(mouse, (Rectangle) {position.x, position.y, size.x, size.y});

        if(hover && (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))){
            newPointToDrag = i;
        }
        DrawRectangleV(position, size, hover ? BLUE : RED);

    }

    if (pointDragging == -1 && newPointToDrag != -1) {
        pointDragging = newPointToDrag;
    }

    if (pointDragging >= 0 && IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
        pointDragging = -1;
    }

    if(pointDragging >= 0){
        points[pointDragging] = mouse;
        Vector2 size = {20, 20};

        Rectangle draggedPoint = {points[pointDragging].x - size.x / 2, points[pointDragging].y - size.y / 2, size.x, size.y};

        for(size_t j = 0; j < points.size(); j++){
            if((size_t)pointDragging == j){
                continue;
            }

            Rectangle otherRec = {points[j].x - size.x/2, points[j].y - size.y / 2, size.x, size.y};
            
            if(CheckCollisionRecs(draggedPoint, otherRec)){
                points[pointDragging] = points[j];
                break;
            }
        }

    } else { 
        if(newPointToDrag == -1 && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
            points.push_back(mouse);
        }
    }

}

void displayQuadraticCurves(){
    DrawTextEx(GetFontDefault(), "QUADRATIC CURVES", {10, 10}, 20, 2, RAYWHITE);
    int n = 20;
    for(size_t i = 0; i + 2 <= points.size(); i += 2){
        Vector2 p1 = points[i];
        Vector2 p2 = points[i + 1];
        Vector2 p3 = points[(i + 2)%points.size()];

        // float dx12 = p2.x - p1.x;
        // float dx23 = p3.x - p2.x;
        // float dy12 = p2.y - p1.y;
        // float dy23 = p3.y - p2.y; 

        for(int j = 0; j <= n; j++){
            float t = (float) j / n;
            Vector2 position = Vector2Lerp(Vector2Lerp(p1, p2, t), Vector2Lerp(p2, p3, t), t);
            Vector2 size = {10, 10};
            // float x = (dx23 - dx12)*t*t + 2*dx12*t + p1.x;
            // float y = (dy23 - dy12)*t*t + 2*dy12*t + p1.y;
            // Vector2 position = {x, y};
            position = Vector2Subtract(position, Vector2Scale(size, 0.5));
            DrawRectangleV(position, size, BLUE);
        }
    }
}

void displayCubicCurves(){
    DrawTextEx(GetFontDefault(), "CUBIC CURVES", {10, 10}, 20, 2, RAYWHITE);
    int n = 20;
    for(size_t i = 0; i + 3 <= points.size(); i += 3){
        Vector2 p1 = points[i];
        Vector2 p2 = points[i + 1];
        Vector2 p3 = points[i + 2];
        Vector2 p4 = points[(i + 3)%points.size()];

        // float dx12 = p2.x - p1.x;
        // float dx23 = p3.x - p2.x;
        // float dx34 = p4.x - p3.x;

        // float dy12 = p2.y - p1.y;
        // float dy23 = p3.y - p2.y;
        // float dy34 = p4.y - p3.y;

        for(int j = 0; j <= n; j++){
            float t = (float) j / n;
            Vector2 pos1 = Vector2Lerp(Vector2Lerp(p1, p2, t), Vector2Lerp(p2, p3, t), t);
            Vector2 pos2 = Vector2Lerp(Vector2Lerp(p2, p3, t), Vector2Lerp(p3, p4, t), t);
            Vector2 finalPoint = Vector2Lerp(pos1, pos2, t);

            // float x = (dx12 - 2*dx23 + dx34)*t*t*t - 3*t*t*(dx12 - dx23) + 3*dx12*t + p1.x;
            // float y = (dy12 - 2*dy23 + dy34)*t*t*t - 3*t*t*(dy12 - dy23) + 3*dy12*t + p1.y;
            // Vector2 finalPoint = {x, y};
            Vector2 size = {10, 10};
            finalPoint = Vector2Subtract(finalPoint, Vector2Scale(size, 0.5));
            DrawRectangleV(finalPoint, size, BLUE);
        }
    }
}

void displayHelp(){
    Vector2 strSize = MeasureTextEx(GetFontDefault(), "[F3] - display quadratic curves", 10, 2);
    const char* str = "[LEFT mb] - click to add points\n[F1] - clear screen\n[F2] - display quadratic curves\n[F3] - display qubic curves\n[F4] - display polygons\n[F5] - display quadratic spline\n[F6] - display cubic spline\n[F7] - display filled polygon\n[U] - undo point placement\n[ESC] - close the screen\n[H] - hide help";
    DrawTextEx(GetFontDefault(), str, {800 - strSize.x - 10, 10}, 10, 2, RAYWHITE);
}

void displayH(){
    const char* str = "[H] - help";
    Vector2 strSize = MeasureTextEx(GetFontDefault(), str, 10, 2);
    DrawTextEx(GetFontDefault(), str, {800 - strSize.x - 10, 10}, 10, 2, RAYWHITE);
}

// This is the first approach by using the winding number alogrihtm.
// To increase/decrease the winding based on the derivative of the curve.
// I's not good as it only works in clockwise point position
void displayFilledCurves(){
    DrawTextEx(GetFontDefault(), "SPLINE", {10, 10}, 20, 2, RAYWHITE);

    for(int row = 0; row < gridHeight; row++){
        int windingNum = 0;
        for(int col = 0; col < gridWidth; col++){
            // float x = (col + 0.5) * cellWidth;
            float y = (row + 0.5) * cellHeight;
            for(size_t i = 0; i + 2 <= points.size(); i+=2){
                Vector2 p1 = points[i];
                Vector2 p2 = points[i + 1];
                Vector2 p3 = points[(i + 2)%points.size()];

                float dx12 = p2.x - p1.x;
                float dx23 = p3.x - p2.x;
                float dy12 = p2.y - p1.y;
                float dy23 = p3.y - p2.y;
                
                float a = dy23 - dy12;
                float b = 2*dy12;
                float c = p1.y - y;
                float D = b*b - 4*a*c;

                if(D < 0.0) continue;

                float t[2];
                t[0] = (-b + sqrt(D)) / (2*a);
                t[1] = (-b - sqrt(D)) / (2*a);

                for(int j = 0; j < 2; j++){
                    if(!(0 <= t[j] && t[j] <= 1)) continue;
                    float tx = (dx23 - dx12)*t[j]*t[j] + 2*dx12*t[j] + p1.x;
                    if(col*cellWidth <= tx && tx <= (col + 1)*cellWidth){
                        // grid[row][col] = true;
                        float d = (dy23 - dy12)*t[j] + dy12;
                        if(d < 0){
                            windingNum += 1;
                        }else{
                            windingNum -= 1;
                        }
                    }
                }


            }
            if(windingNum > 0){
                grid[row][col] = true;
            }else{
                grid[row][col] = false;
            }
        }
    }

    for(int i = 0; i < gridHeight; i++){
        for(int j = 0; j < gridWidth; j++){
            if(grid[i][j]){
                Vector2 markerPosition = {(float)j * cellWidth, (float)i * cellHeight};
                Vector2 cellSize = {(float)cellWidth, (float)cellHeight};
                Vector2 markerSize = Vector2Scale(cellSize, 0.4);
                markerPosition = Vector2Add(markerPosition, Vector2Scale(cellSize, 0.5));
                markerPosition = Vector2Subtract(markerPosition, Vector2Scale(markerSize, 0.5));
                DrawRectangleV(markerPosition, markerSize, RED);
            }
        }
    }
}

int cmpSolutions(const void* s1, const void* s2){
    const Solution* a = static_cast<const Solution*> (s1);
    const Solution* b = static_cast<const Solution*> (s2);

    if(a->tx < b->tx){
        return -1;
    }else if(a->tx > b->tx){
        return 1;
    }else{
        return 0;
    }
}

void solveRow(int row, vector<Solution>& solutions){
    solutions.clear();
    float y = (row + 0.5) * cellHeight;
    for(size_t i = 0; i + 2 <= points.size(); i+=2){
        Vector2 p1 = points[i];
        Vector2 p2 = points[i + 1];
        Vector2 p3 = points[(i + 2)%points.size()];

        float dx12 = p2.x - p1.x;
        float dx23 = p3.x - p2.x;
        float dy12 = p2.y - p1.y;
        float dy23 = p3.y - p2.y;
        
        float a = dy23 - dy12;
        float b = 2*dy12;
        float c = p1.y - y;

        float t[2];
        int tn = 0;

        if(abs(a) > 1e-6){
            float D = b*b - 4*a*c;
            if(D >= 0.0){
                t[tn++] = (-b + sqrt(D)) / (2*a);
                t[tn++] = (-b - sqrt(D)) / (2*a);
            }
        }else if(abs(b) > 1e-6){
            t[tn++] = -c/b;
        }

        for(int j = 0; j < tn; j++){
            if(!(0 <= t[j] && t[j] <= 1)) continue;
            float tx = (dx23 - dx12)*t[j]*t[j] + 2*dx12*t[j] + p1.x;
            float d = (dy23 - dy12)*t[j] + dy12;
            Solution s = {tx, d};
            solutions.push_back(s);
        }
    }
    qsort(solutions.data(), solutions.size(), sizeof(Solution), cmpSolutions);
}

// Better option by iterating over the grid and solving the derivatives
// for each x, then sort them by x
void displayFilledCurvesImproved(){
    DrawTextEx(GetFontDefault(), "QUADRATIC SPLINE", {10, 10}, 20, 2, RAYWHITE);
    for(size_t row = 0; row < gridHeight; row++){
        for(size_t col = 0; col < gridWidth; col++){
            grid[row][col] = false;
        }
    }
    for(size_t row = 0; row < gridHeight; row++){
        int windingNum = 0;
        solveRow(row, solutions);
        for(size_t i = 0; i < solutions.size(); i++){
            Solution s = solutions[i];
            if(windingNum > 0){
                if(i > 0){
                    Solution p = solutions[i - 1];
                    size_t col1 = p.tx / cellWidth;
                    size_t col2 = s.tx / cellWidth;
                    for(size_t col = col1; col <= col2; col++){
                        grid[row][col] = true;
                    }
                }
            }
            if(s.d < 0){
                windingNum += 1;
            }else if(s.d > 0){
                windingNum -= 1;
            }
        }
    }

    for(int i = 0; i < gridHeight; i++){
        for(int j = 0; j < gridWidth; j++){
            if(grid[i][j]){
                Vector2 markerPosition = {(float)j * cellWidth, (float)i * cellHeight};
                Vector2 cellSize = {(float)cellWidth, (float)cellHeight};
                Vector2 markerSize = Vector2Scale(cellSize, 0.4);
                markerPosition = Vector2Add(markerPosition, Vector2Scale(cellSize, 0.5));
                markerPosition = Vector2Subtract(markerPosition, Vector2Scale(markerSize, 0.5));
                DrawRectangleV(markerPosition, markerSize, RED);
            }
        }
    }
}

// AI generated section along with the results in the next function, since mine was way to buggy    
/*---------------------------------------------------------------------------------------------------------------------*/
    // tolerance for comparing to 0 and for deduplicating roots
    static constexpr long double EPS = 1e-12L;

    vector<long double> unique_sorted_real(vector<long double>& v, long double tol = 1e-9L) {
        sort(v.begin(), v.end());
        vector<long double> out;
        for (auto x : v) {
            if (out.empty() || fabsl(x - out.back()) > tol) out.push_back(x);
        }
        return out;
    }

    vector<long double> solve_quadratic_real(long double a, long double b, long double c) {
        vector<long double> roots;
        if (fabsl(a) < EPS) {
            // linear bx + c = 0
            if (fabsl(b) > EPS) roots.push_back(-c / b);
            return roots;
        }
        long double disc = b*b - 4*a*c;
        if (disc < -EPS) return roots; // no real roots
        if (fabsl(disc) <= EPS) {
            roots.push_back(-b / (2*a));
        } else {
            long double s = sqrt((long double)disc);
            roots.push_back((-b - s) / (2*a));
            roots.push_back((-b + s) / (2*a));
        }
        return unique_sorted_real(roots);
    }
/*---------------------------------------------------------------------------------------------------------------------*/

void solveRowCubic(int row, vector<Solution>& solutions){
    solutions.clear();

    float y = (row + 0.5) * cellHeight;
    for(size_t i = 0; i + 3 <= points.size(); i += 3){
        Vector2 p1 = points[i];
        Vector2 p2 = points[i + 1];
        Vector2 p3 = points[i + 2];
        Vector2 p4 = points[(i + 3)%points.size()];

        double dx12 = p2.x - p1.x;
        double dx23 = p3.x - p2.x;
        double dx34 = p4.x - p3.x;

        double dy12 = p2.y - p1.y;
        double dy23 = p3.y - p2.y;
        double dy34 = p4.y - p3.y;

        long double a = dy12 - 2*dy23 + dy34;
        long double b = (-3)*dy12 + 3*dy23;
        long double c = 3*dy12;
        long double d = p1.y - y;

        vector<long double> roots;

        // handle not cubic
        if (fabsl(a) < EPS) {
            roots = solve_quadratic_real(b, c, d);
        }

        float minY = std::min({p1.y, p2.y, p3.y, p4.y});
        float maxY = std::max({p1.y, p2.y, p3.y, p4.y});
        if (y < minY || y > maxY) continue; // skip this segment

        
        // normalize: x^3 + A x^2 + B x + C = 0
        long double A = b / a;
        long double B = c / a;
        long double C = d / a;

        // depressed cubic substitution x = y - A/3 => y^3 + p*y + q = 0
        long double A2 = A*A;
        long double p = B - A2 / 3.0L;
        long double q = (2.0L*A*A2) / 27.0L - (A*B) / 3.0L + C;

        long double Q = p / 3.0L;
        long double R = q / 2.0L;
        long double discriminant = R*R + Q*Q*Q; // (q/2)^2 + (p/3)^3

        if (discriminant > EPS) {
            // one real root
            long double sqrtD = sqrt((long double)discriminant);
            long double u = -R + sqrtD;
            long double v = -R - sqrtD;
            // use std::cbrt to handle negative values properly
            long double u_c = cbrt(u);
            long double v_c = cbrt(v);
            long double y = u_c + v_c;
            long double x = y - A / 3.0L;
            roots.push_back(x);
        } else if (fabsl(discriminant) <= EPS) {
            // discriminant == 0: multiple roots, all real
            // double (or triple) root(s)
            long double u_c = cbrt(-R);
            long double y1 = 2.0L*u_c;
            long double y2 = -u_c;
            roots.push_back(y1 - A / 3.0L);
            roots.push_back(y2 - A / 3.0L);
            // if triple root u_c==0 then both are same
        } else {
            // three distinct real roots
            long double rho = sqrt(-Q); // sqrt(-p/3)
            long double phi = acosl( max((long double)-1.0L, min((long double)1.0L, -R / (rho*rho*rho))) );
            long double two_rho = 2.0L * rho;
            long double y0 = two_rho * cosl(phi / 3.0L);
            long double y1 = two_rho * cosl((phi + 2.0L*M_PI) / 3.0L);
            long double y2 = two_rho * cosl((phi + 4.0L*M_PI) / 3.0L);
            roots.push_back(y0 - A / 3.0L);
            roots.push_back(y1 - A / 3.0L);
            roots.push_back(y2 - A / 3.0L);
        }     

        // filter real (they already are real), remove NaNs and duplicates, sort
        vector<long double> real_roots;
        for (auto r : roots) {
            if (!isnan((double)r) && isfinite((double)r)) real_roots.push_back(r);
        }
        roots = unique_sorted_real(real_roots, 1e-9L);   

        for(size_t j = 0; j < roots.size(); j++){
            if(!(0 <= roots[j] && roots[j] <= 1)) continue;
            float tx = (dx12 - 2*dx23 + dx34)*roots[j]*roots[j]*roots[j] - 3*roots[j]*roots[j]*(dx12 - dx23) + 3*dx12*roots[j] + p1.x;
            float d = 3*(dy12*(roots[j] - 1)*(roots[j] - 1) + roots[j]*(dy34*roots[j] - 2*dy23*(roots[j] - 1)));
            // cout << "We have solution : [" << tx << " " << d << "]"<< endl;
            Solution s = {tx, d};
            solutions.push_back(s);
        }

    }

    /* - Buggy code, cannot find the bug:

        float bn;
        float cn;
        float dn;

        // normalze roots
        if(abs(a) < 1e-6){
            bn = b;
            cn = c;
            dn = d;
        }else{
            bn = b / a;
            cn = c / a;
            dn = d / a;
        }

        // depressed cubic substitution
        float b_3 = bn / 3;
        float p = cn - (bn * bn) / 3.0f;
        float q = (2.0 * bn * bn * bn) / 27.0 - ((bn * cn) / 3.0) + dn;

        // discriminant
        float disc = (q * q) / 4.0 + (p*p*p) / 27.0;

        vector<float> tn;
        if(disc < 1e-6) disc = 0.0;

        // One real solution
        if(disc > 0.0){
            float sqDisc = sqrt(disc);
            float u = cbrt(-q / 2.0 + sqDisc);
            float v = cbrt(-q / 2.0 - sqDisc);
            float yn = u + v;
            tn.push_back(yn - b_3);
        }else if(disc == 0.0){
            // Multiple solutions, at least a double
            float u = cbrt(-q / 2.0);
            float y1 = u * 2;
            float y2 = -u;
            tn.push_back(y1 - b_3);
            tn.push_back(y2 - b_3);

            // Remove duplicates within tolerance
            sort(tn.begin(), tn.end());
            tn.erase(unique(tn.begin(), tn.end(), [](float a, float b){return abs(a - b) < 1e-6;}), tn.end());
        }else{
            // Three distinct real roots (disc < 0)
            double r = sqrt(-p * p * p / 27.0);
            double phi = acos(clamp(-q / (2.0 * r), -1.0, 1.0));
            double m = 2.0 * sqrt(-p / 3.0);

            tn.push_back(m * cos(phi / 3.0) - b_3);
            tn.push_back(m * cos((phi + 2.0 * PI) / 3.0) - b_3);
            tn.push_back(m * cos((phi + 4.0 * PI) / 3.0) - b_3);
            sort(tn.begin(), tn.end());
        }

        for(size_t j = 0; j < tn.size(); j++){
            if(!(0 <= tn[j] && tn[j] <= 1)) continue;
            float tx = (dx12 - 2*dx23 + dx34)*tn[j]*tn[j]*tn[j] - 3*tn[j]*tn[j]*(dx12 - dx23) + 3*dx12*tn[j] + p1.x;
            float d = 3*(dy12*(tn[j] - 1)*(tn[j] - 1) + tn[j]*(dy34*tn[j] - 2*dy23*(tn[j] - 1)));
            Solution s = {tx, d};
            solutions.push_back(s);
        }
    }
    qsort(solutions.data(), solutions.size(), sizeof(Solution), cmpSolutions); 
    */
}


void displayFilledCurvesCubic(){
    DrawTextEx(GetFontDefault(), "CUBIC SPLINE", {10, 10}, 20, 2, RAYWHITE);
    for(size_t row = 0; row < gridHeight; row++){
        for(size_t col = 0; col < gridWidth; col++){
            grid[row][col] = false;
        }
    }
    for(size_t row = 0; row < gridHeight; row++){
        int windingNum = 0;
        solveRowCubic(row, solutions);
        for(size_t i = 0; i < solutions.size(); i++){
            Solution s = solutions[i];
            // cout << s.tx << " " << s.d << endl;
            if(windingNum > 0){
                if(i > 0){
                    Solution p = solutions[i - 1];
                    size_t col1 = p.tx / cellWidth;
                    size_t col2 = s.tx / cellWidth;
                    for(size_t col = col1; col <= col2; col++){
                        grid[row][col] = true;
                    }
                }
            }
            if(s.d < 0){
                windingNum += 1;
            }else if(s.d > 0){
                windingNum -= 1;
            }
        }
    }

    for(int i = 0; i < gridHeight; i++){
        for(int j = 0; j < gridWidth; j++){
            if(grid[i][j]){
                Vector2 markerPosition = {(float)j * cellWidth, (float)i * cellHeight};
                Vector2 cellSize = {(float)cellWidth, (float)cellHeight};
                Vector2 markerSize = Vector2Scale(cellSize, 0.4);
                markerPosition = Vector2Add(markerPosition, Vector2Scale(cellSize, 0.5));
                markerPosition = Vector2Subtract(markerPosition, Vector2Scale(markerSize, 0.5));
                DrawRectangleV(markerPosition, markerSize, RED);
            }
        }
    }
}


void displayPolygons(){
    DrawTextEx(GetFontDefault(), "POLYGONS", {10, 10}, 20, 2, RAYWHITE);
    int n = 20;
    for(size_t i = 0; i+1 <= points.size(); i++){
        Vector2 p1 = points[i];
        Vector2 p2 = points[(i + 1)%points.size()];

        float dx12 = p2.x - p1.x;
        float dy12 = p2.y - p1.y;

        for(int j = 0; j <= n; j++){
            float t = (float) j / n;
            float x = dx12*t + p1.x;
            float y = dy12*t + p1.y;
            Vector2 size = {10, 10};
            Vector2 position = {x, y};
            position = Vector2Subtract(position, Vector2Scale(size, 0.5));
            DrawRectangleV(position, size, BLUE);
        }
    }
}

void solveRowPoly(int row, vector<Solution> solutions){

}

void displayFilledPolygons(){
    DrawTextEx(GetFontDefault(), "FILLED POLYGONS", {10, 10}, 20, 2, RAYWHITE);
    for(size_t row = 0; row < gridHeight; row++){
        for(size_t col = 0; col < gridWidth; col++){
            grid[row][col] = false;
        }
    }
    for(size_t row = 0; row < gridHeight; row++){
        int windingNumber = 0;
        solveRowPoly(row, solutions);
        for(size_t i = 0; i < solutions.size(); i++){
            Solution s = solutions[i];
            cout << s.tx << endl;
        }
    }
}

int main(){

    // SetConfigFlags(FLAG_BORDERLESS_WINDOWED_MODE | FLAG_WINDOW_UNDECORATED);
    InitWindow(windowWidth, windowHeight, "berzier curves");
    SetTargetFPS(60);

    bool showQuadraticCurves = false;
    bool showCubicCurves = false;
    bool showPolygons = false;
    bool showHelp = false;
    bool showAreaWithinQuad = false;
    bool showAreaWithinCub = false;
    bool showAreaWithinPolygon = false;

    while(!WindowShouldClose()){
        // F1 to clear the screen
        if(IsKeyPressed(KEY_F1)){
            points.clear();
            showQuadraticCurves = false;
            showCubicCurves = false;
            showAreaWithinQuad = false;
            showAreaWithinCub = false;
            showPolygons = false;
        }else if(IsKeyPressed(KEY_F2)){
            showQuadraticCurves = !showQuadraticCurves;
            showCubicCurves = false;
            showAreaWithinQuad = false;
            showAreaWithinCub = false;
            showPolygons = false;
        }else if(IsKeyPressed(KEY_F3)){
            showQuadraticCurves = false;
            showAreaWithinQuad = false;
            showAreaWithinCub = false;
            showPolygons = false;
            showCubicCurves = !showCubicCurves;
        }else if(IsKeyPressed(KEY_F5)){
            showQuadraticCurves = false;
            showCubicCurves = false;
            showAreaWithinCub = false;
            showPolygons = false;
            showAreaWithinQuad = !showAreaWithinQuad;
            for(int y = 0; y < gridHeight; y++){
                for(int x = 0; x < gridWidth; x++){
                    grid[y][x] = false;
                }
            }
        }else if(IsKeyPressed(KEY_F6)){
            showQuadraticCurves = false;
            showCubicCurves = false;
            showAreaWithinQuad = false;
            showPolygons = false;
            showAreaWithinCub = !showAreaWithinCub;
            for(int y = 0; y < gridHeight; y++){
                for(int x = 0; x < gridWidth; x++){
                    grid[y][x] = false;
                }
            }
        }else if(IsKeyPressed(KEY_F4)){
            showQuadraticCurves = false;
            showCubicCurves = false;
            showAreaWithinQuad = false;
            showAreaWithinCub = false;
            showPolygons = !showPolygons;
        }else if(IsKeyPressed(KEY_F7)){
            showQuadraticCurves = false;
            showCubicCurves = false;
            showPolygons = false; 
            showAreaWithinQuad = false;
            showAreaWithinCub = false;
            showAreaWithinPolygon = !showAreaWithinPolygon;
        }
        // undo last placement
        if(IsKeyPressed(KEY_U)){
            if(points.size() > 0){
                points.erase(points.end());
            }
        }
        // help
        if(IsKeyPressed(KEY_H)){
            showHelp = !showHelp;
        }
        BeginDrawing();
            ClearBackground(GetColor(0x181818FF));
            displayPoints();
            if(showQuadraticCurves){
                displayQuadraticCurves();
            }else if(showCubicCurves){
                displayCubicCurves();
            }else if(showAreaWithinCub){
                displayFilledCurvesCubic();
            }else if(showAreaWithinQuad){
                displayFilledCurvesImproved();
            }else if(showPolygons){
                displayPolygons();
            }else if(showAreaWithinPolygon){
                displayFilledPolygons();
            }
            if(showHelp){
                displayHelp();
            }else{
                displayH();
            }
        EndDrawing();
    }

    return 0;
}