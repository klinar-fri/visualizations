#include "raylib.h"
#include "raymath.h"
#include <vector>
using namespace std;

vector<Vector2> points;
int pointDragging = -1;

int heightFactor = 4;
int widthFactor = 3;
int windowSize = 200;

int windowHeight = heightFactor * windowSize;
int windowWidth = widthFactor * windowSize;

int gridFactor = 10;
int gridWidth = gridFactor * widthFactor;
int gridHeight = gridFactor * heightFactor;

int cellWidth = windowWidth / gridWidth;
int cellHeight = windowHeight / gridHeight;

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

    if(pointDragging >= 0){
        points[pointDragging] = mouse;
        
        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
            pointDragging = -1;
        }
    } 
    else { 
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
            points.push_back(mouse);
        }
    }

}

void displayQuadraticCurves(){
    DrawTextEx(GetFontDefault(), "QUADRATIC CURVES", {10, 10}, 20, 2, RAYWHITE);
    int n = 20;
    for(size_t i = 0; i + 2 < points.size(); i += 2){
        Vector2 p1 = points[i];
        Vector2 p2 = points[i + 1];
        Vector2 p3 = points[i + 2];
        for(int j = 0; j <= n; j++){
            float t = (float) j / n;
            Vector2 position = Vector2Lerp(Vector2Lerp(p1, p2, t), Vector2Lerp(p2, p3, t), t);
            Vector2 size = {10, 10};
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
    for(int i = 0; i < gridHeight; i++){
        for(int j = 0; j < gridWidth; j++){
            DrawRectangle(i*cellWidth, j*cellHeight, cellWidth, cellHeight, (i+j)%2 ? RED : BLACK);
        }
    }
}

int main(){

    SetConfigFlags(FLAG_BORDERLESS_WINDOWED_MODE | FLAG_WINDOW_UNDECORATED);
    InitWindow(windowHeight, windowWidth, "berzier curves");
    SetTargetFPS(60);

    bool showQuadraticCurves = false;
    bool showQubicCurves = false;
    bool showHelp = false;
    bool showAreaWithinQuadratic = false;


    while(!WindowShouldClose()){
        // F1 to clear the screen
        if(IsKeyPressed(KEY_F1)){
            points.clear();
            showQuadraticCurves = false;
            showQubicCurves = false;
        }
        if(IsKeyPressed(KEY_F2)){
            showQuadraticCurves = !showQuadraticCurves;
            showQubicCurves = false;
        }
        if(IsKeyPressed(KEY_F3)){
            showQuadraticCurves = false;
            showQubicCurves = !showQubicCurves;
        }
        if(IsKeyPressed(KEY_F4)){
            showAreaWithinQuadratic = !showAreaWithinQuadratic;
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
            if(showAreaWithinQuadratic){
                displayFilledCurves();
            }
            displayPoints();
            if(showQuadraticCurves){
                displayQuadraticCurves();
            }else if(showQubicCurves){
                displayQubicCurves();
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