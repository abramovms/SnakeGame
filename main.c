#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>

#define _WIN32_WINNT 0x0A00

typedef struct TPoint {
    float x;
    float y;
}Point;

Point InPoint(float x, float y) {
    Point pt;
    pt.x = x;
    pt.y = y;
    return pt;
}

typedef struct TObject {
    Point pos;
    Point size;
    Point speed;
    COLORREF brush;
    char oType;
}AObject, *PObject;

AObject apple;
PObject snake = NULL;
int Cant = 2;

void NewPartSnake() {
    Cant++;
    snake = (PObject) realloc(snake, Cant * sizeof(AObject));
}

void NewApple() {
    int xPos, yPos;
    xPos = rand() % 600;
    yPos = rand() % 400;
    apple.pos = InPoint(xPos, yPos);
}

void DelSnakeParts() {
    snake = (PObject) realloc(snake, Cant * sizeof(AObject));
}

BOOL ObjectCollision(AObject o1, AObject o2) {
    return ((o1.pos.x + o1.size.x) > o2.pos.x) && (o1.pos.x < (o2.pos.x + o2.size.x)) &&
            ((o1.pos.y + o1.size.y) > o2.pos.y) && (o1.pos.y < (o2.pos.y + o2.size.y));
}

void CheckChangeEdge() {
    int i;
    for (i = 0; i < Cant; i++) {
        if (snake[i].pos.x + snake[i].size.x > 630) {
            if (i == 0) snake[0].pos.x = 5;
            if (!snake[0].speed.x) {
                snake[0].speed.y = (snake[0].speed.y < 0) ? -20 : 20;
            }
            else snake[0].speed.x = (snake[0].speed.x < 0) ? -20 : 20;
            if (!CheckAngle()) Sleep(50);
            return;
        }
    }
    for (i = 0; i < Cant; i++) {
        if (snake[i].pos.x < 5) {
            if (i == 0) snake[0].pos.x = 610;
            if (!snake[0].speed.x) {
                snake[0].speed.y = (snake[0].speed.y < 0) ? -20 : 20;
            }
            else snake[0].speed.x = (snake[0].speed.x < 0) ? -20 : 20;
            if (!CheckAngle()) Sleep(50);
            return;
        }
    }
    for (i = 0; i < Cant; i++) {
        if (snake[i].pos.y + snake[i].size.y > 430) {
            if (i == 0) snake[0].pos.y = 20;
            if (!snake[0].speed.x) {
                snake[0].speed.y = (snake[0].speed.y < 0) ? -20 : 20;
            }
            else snake[0].speed.x = (snake[0].speed.x < 0) ? -20 : 20;
            if (!CheckAngle()) Sleep(50);
            return;
        }
    }
    for (i = 0; i < Cant; i++) {
        if (snake[i].pos.y < 20) {
            if (i == 0) snake[0].pos.y = 410;
            if (!snake[0].speed.x) {
                snake[0].speed.y = (snake[0].speed.y < 0) ? -20 : 20;
            }
            else snake[0].speed.x = (snake[0].speed.x < 0) ? -20 : 20;
            if (!CheckAngle()) Sleep(50);
            return;
        }
    }
}

BOOL CheckAngle() {
    int i;
    for (i = 1; i < Cant; i++) {
        if (!snake[0].speed.x) {
                if (snake[i].pos.x != snake[0].pos.x) return TRUE;
                continue;
        }
        if (snake[i].pos.y != snake[0].pos.y) return TRUE;
    }
    return FALSE;
}

void InitObject(AObject *obj, float xPos, float yPos, float width, float height, COLORREF brush, char type) {
    obj->pos = InPoint(xPos, yPos);
    obj->size = InPoint(width, height);
    if (type == 'r') obj->speed = InPoint(0,0);
    else obj->speed = InPoint(4, 0);
    obj->brush = brush;
    obj->oType = type;
}

void ShowObject(AObject obj, HDC dc) {
    SelectObject(dc, GetStockObject(DC_BRUSH));
    SetDCBrushColor(dc, obj.brush);
    SelectObject(dc, GetStockObject(DC_PEN));
    SetDCPenColor(dc, RGB(0, 0, 0));
    if (obj.oType == 'r') {
        Ellipse(dc, (int)obj.pos.x, (int)obj.pos.y, (int)(obj.pos.x + obj.size.x), (int)(obj.pos.y + obj.size.y));
    }
    else Rectangle(dc, (int)obj.pos.x, (int)obj.pos.y, (int)(obj.pos.x + obj.size.x), (int)(obj.pos.y + obj.size.y));
}

void MoveObject() {
    int i;

    if (abs(snake[0].speed.x + snake[0].speed.y) == 4) {
        for (i = 0; i < Cant; i++) {
            snake[i].pos.x += snake[i].speed.x;
            snake[i].pos.y += snake[i].speed.y;
        }
    }
    else {
        float kx, ky;
        float dx = snake[0].pos.x;
        float dy = snake[0].pos.y;
        snake[0].pos.x += snake[0].speed.x;
        snake[0].pos.y += snake[0].speed.y;
        for (i = 1; i < Cant; i++) {
            if (i % 2 == 0) {
                dx = snake[i].pos.x;
                dy = snake[i].pos.y;
                snake[i].pos.x = kx;
                snake[i].pos.y = ky;
                continue;
            }
            kx = snake[i].pos.x;
            ky = snake[i].pos.y;
            snake[i].pos.x = dx;
            snake[i].pos.y = dy;
        }
        if (!CheckAngle()) {
                if (!snake[0].speed.x) snake[0].speed.y = (snake[0].speed.y < 0) ? -4 : 4;
                else snake[0].speed.x = (snake[0].speed.x < 0) ? -4 : 4;
        }
    }
    if (ObjectCollision(apple, snake[0])) {
        float x, y;
        NewApple();
        if (CheckAngle()) {
            NewPartSnake();
            x = snake[Cant - 2].pos.x;
            y = snake[Cant - 2].pos.y;
        }
        else {
            NewPartSnake();
            if (!snake[Cant - 2].speed.x) {
                if (snake[Cant - 2].speed.y > 0) {
                    x = snake[Cant - 2].pos.x;
                    y = snake[Cant - 2].pos.y - 20;
                }
                else {
                    x = snake[Cant - 2].pos.x;
                    y = snake[Cant - 2].pos.y + 20;
                }
            }
            else {
                if (snake[Cant - 2].speed.x > 0) {
                    x = snake[Cant - 2].pos.x - 20;
                    y = snake[Cant - 2].pos.y;
                }
                else {
                    x = snake[Cant - 2].pos.x + 20;
                    y = snake[Cant - 2].pos.y;
                }
            }
        }

        InitObject(&snake[Cant - 1], x, y, 20,20, RGB(0,255,0), 'g');
        snake[Cant - 1].speed = snake[Cant - 2].speed;
    }
    CheckChangeEdge();
    for (i = 3; i < Cant; i++) {
        if (ObjectCollision(snake[0], snake[i])) {
            Cant = i;
            DelSnakeParts();
            break;
        }
    }
}

void PlayerControl() {
    static int speed = 4;
    static int angle = 20;
    int i;
    if (GetKeyState('W') < 0 && snake[i].speed.y == 0) {
            snake[0].speed = InPoint(0, -angle);
            for (i = 1; i < Cant; i++) {
                snake[i].speed = InPoint(0, -speed);
            }
            return;
    }
    if (GetKeyState('S') < 0 && snake[i].speed.y == 0) {
            snake[0].speed = InPoint(0, angle);
            for (i = 1; i < Cant; i++) {
                snake[i].speed = InPoint(0, speed);
            }
            return;
    }
    if (GetKeyState('A') < 0 && snake[i].speed.x == 0) {
            snake[0].speed = InPoint(-angle, 0);
            for (i = 1; i < Cant; i++) {
                snake[i].speed = InPoint(-speed, 0);
            }
            return;
    }
    if (GetKeyState('D') < 0 && snake[i].speed.x == 0) {
            snake[0].speed = InPoint(angle, 0);
            for (i = 1; i < Cant; i++) {
                snake[i].speed = InPoint(speed, 0);
            }
            return;
    }
}

RECT rct;

void WinInit() {
    srand(time(NULL));
    int i;
    NewPartSnake();
    for (i = 0; i < Cant; i++) {
        InitObject(snake + i, 100 - 20 * i,100, 20,20, RGB(0, 255, 0), 'g');
    }
    InitObject(&apple, rand() % 600, rand() % 400, 20,20, RGB(255, 0, 0), 'r');
}

void WinMove() {
    PlayerControl();
    MoveObject();
}

void WinShow(HDC dc) {

    HDC memDC = CreateCompatibleDC(dc);
    HBITMAP memBM = CreateCompatibleBitmap(dc, rct.right - rct.left, rct.bottom - rct.top);
    SelectObject(memDC, memBM);

    int i;

    SelectObject(memDC, GetStockObject(DC_BRUSH));
    SetDCBrushColor(memDC, RGB(255,255,255));
    SelectObject(memDC, GetStockObject(DC_PEN));
    SetDCPenColor(memDC, RGB(255,255,255));
    Rectangle(memDC, 0,0, 640,480);

    for (i = 0; i < Cant; i++) {
        ShowObject(snake[i], memDC);
    }

    ShowObject(apple, memDC);

    BitBlt(dc, 0,0, rct.right - rct.left, rct.bottom - rct.top, memDC, 0,0, SRCCOPY);
    DeleteDC(memDC);
    DeleteObject(memBM);
}

LRESULT WindProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam) {
    if (message == WM_DESTROY) PostQuitMessage(0);
    else if (message == WM_KEYDOWN) printf("code = %d\n", wparam);
    else if (message == WM_CHAR) printf("%c\n", wparam);
    else if (message == WM_SIZE) {
        GetClientRect(hwnd, &rct);
    }
    else if (message == WM_MOUSEMOVE) {
        int xPos = LOWORD(lparam);
        int yPos = HIWORD(lparam);
        printf("%d %d\n", xPos, yPos);
    }
    else if(message == WM_LBUTTONDOWN) {
        printf("Mouse button pick\n");
    }
    else return DefWindowProcA(hwnd, message, wparam, lparam);
}

int main() {

    WNDCLASSA wcl;

    memset(&wcl, 0, sizeof(WNDCLASSA));
    wcl.lpszClassName = "my Window";
    wcl.lpfnWndProc = (WNDPROC)WindProc;

    RegisterClassA(&wcl);

    HWND hwnd;

    hwnd = CreateWindow("my Window", "Window Name", WS_OVERLAPPEDWINDOW, 10,10, 640,480, NULL, NULL, NULL, NULL);

    HDC dc = GetDC(hwnd);

    ShowWindow(hwnd, SW_SHOWNORMAL);

    WinInit();

    MSG msg;
    while (1) {
            if (PeekMessageA(&msg, NULL, 0, 0, PM_REMOVE)) {
                if (msg.message == WM_QUIT) break;
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
            WinMove();
            WinShow(dc);
            if (CheckAngle()) Sleep(50);
            else Sleep(10);
    }

    free(snake);

    return 0;
}
