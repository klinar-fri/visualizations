#include "raylib.h"
#include "raymath.h"
#include <vector>
using namespace std;


vector<Vector2> points;
int pointDragging = -1;

void displayPoints(){
    Vector2 mouse = GetMousePosition();

    for(size_t i = 0; i < points.size(); i++){
        Vector2 size = {20, 20};
        Vector2 position = points[i];
        position = Vector2Subtract(position, Vector2Scale(size, 0.5));
        bool hover = CheckCollisionPointRec(mouse, (Rectangle) {position.x, position.y, size.x, size.y});

        if(hover){
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) pointDragging = i;
        }else{
            if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) pointDragging = -1;
        }
        DrawRectangleV(position, size, hover ? PURPLE : RED);

    }

    if(pointDragging >= 0){
        points[pointDragging] = mouse;
    }else{
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

int main(){

    int windowWidth = 800;
    int windowHeight = 600;

    SetConfigFlags(FLAG_BORDERLESS_WINDOWED_MODE | FLAG_WINDOW_UNDECORATED);
    InitWindow(windowWidth, windowHeight, "berzier curves");
    SetTargetFPS(60);

    bool showQuadraticCurves = false;
    bool showQubicCurves = false;

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
        // undo last placement
        if(IsKeyPressed(KEY_U)){
            if(points.size() > 0){
                points.erase(points.end());
            }
        }
        BeginDrawing();
            ClearBackground(GetColor(0x181818FF));
            displayPoints();
            if(showQuadraticCurves){
                displayQuadraticCurves();
            }else if(showQubicCurves){
                displayQubicCurves();
            }
        EndDrawing();
    }

    return 0;
}