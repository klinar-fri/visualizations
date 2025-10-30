#include "raylib.h"
#include "raymath.h"
#include <math.h>
#include <iostream>
#include <vector>
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


void displayQubicCurves(){
    DrawTextEx(GetFontDefault(), "CUBIC CURVES", {10, 10}, 20, 2, RAYWHITE);
    int n = 20;
    for(size_t i = 0; i + 3 < points.size(); i += 3){
        Vector2 p1 = points[i];
        Vector2 p2 = points[i + 1];
        Vector2 p3 = points[i + 2];
        Vector2 p4 = points[i + 3];
        for(int j = 0; j <= n; j++){
            float t = (float) j / n;
            Vector2 pos1 = Vector2Lerp(Vector2Lerp(p1, p2, t), Vector2Lerp(p2, p3, t), t);
            Vector2 pos2 = Vector2Lerp(Vector2Lerp(p2, p3, t), Vector2Lerp(p3, p4, t), t);
            Vector2 finalPoint = Vector2Lerp(pos1, pos2, t);
            Vector2 size = {10, 10};
            finalPoint = Vector2Subtract(finalPoint, Vector2Scale(size, 0.5));
            DrawRectangleV(finalPoint, size, BLUE);
        }
    }
}

void displayHelp(){
    Vector2 strSize = MeasureTextEx(GetFontDefault(), "[F3] - display quadratic curves", 10, 2);
    const char* str = "[LEFT mb] - click to add points\n[RIGHT mb] - connect line\n[F1] - clear screen\n[F2] - display quadratic curves\n[F3] - display qubic curves\n[F4] - show area enclosed\n[U] - undo point placement\n[H] - hide help";
    DrawTextEx(GetFontDefault(), str, {800 - strSize.x - 10, 10}, 10, 2, RAYWHITE);
}

void displayH(){
    const char* str = "[H] - help";
    Vector2 strSize = MeasureTextEx(GetFontDefault(), str, 10, 2);
    DrawTextEx(GetFontDefault(), str, {800 - strSize.x - 10, 10}, 10, 2, RAYWHITE);
}

void displayFilledCurves(){
    DrawTextEx(GetFontDefault(), "SPLINE", {10, 10}, 20, 2, RAYWHITE);

    for(int row = 0; row < gridHeight; row++){
        int windingNum = 0;
        for(int col = 0; col < gridWidth; col++){
            float x = (col + 0.5) * cellWidth;
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
                    if(x < tx) continue;
                    if(abs(tx - x) < cellWidth*0.5){
                        // grid[row][col] = true;
                        float d = (dy23 - dy12)*t[j] + dy12;
                        if(d < 0){
                            windingNum += 1;
                        }else if(d > 0){
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

int main(){

    SetConfigFlags(FLAG_BORDERLESS_WINDOWED_MODE | FLAG_WINDOW_UNDECORATED);
    InitWindow(windowWidth, windowHeight, "berzier curves");
    SetTargetFPS(60);

    bool showQuadraticCurves = false;
    bool showQubicCurves = false;
    bool showHelp = false;
    bool showAreaWithin = false;

    while(!WindowShouldClose()){
        // F1 to clear the screen
        if(IsKeyPressed(KEY_F1)){
            points.clear();
            showQuadraticCurves = false;
            showQubicCurves = false;
            showAreaWithin = false;
        }
        if(IsKeyPressed(KEY_F2)){
            showQuadraticCurves = !showQuadraticCurves;
            showQubicCurves = false;
            showAreaWithin = false;
        }
        if(IsKeyPressed(KEY_F3)){
            showQuadraticCurves = false;
            showAreaWithin = false;
            showQubicCurves = !showQubicCurves;
        }
        if(IsKeyPressed(KEY_F4)){
            showQuadraticCurves = false;
            showQubicCurves = false;
            showAreaWithin = !showAreaWithin;
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
            }else if(showQubicCurves){
                displayQubicCurves();
            }else if(showAreaWithin){
                displayFilledCurves();
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