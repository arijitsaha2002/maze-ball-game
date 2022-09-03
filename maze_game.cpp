#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/fl_draw.H>
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <cmath>
using namespace std;

const int Window_W = 1000;
const int Window_H = 1000;
const double a_const = M_PI / (double)180;
const int R0 = 320;
const int R1 = 240;
const int R2 = 160;
const int R3 = 80;
const int GAP = R1 - R2;
const int thikness = 3;
const int color_b = 7;
const int r_ball = 30;
const int a_gap_1 = 0;
const int a_gap_2 = 120;
const int a_gap_3 = 240;
const int color_line = 6;
const int color_end = 14;
const int fric = 10;

double diff(int a, int b)
{
    return acos(cos((a - b) * a_const)) / a_const;
}

void circle(double x, double y, double r, int color = 2, int a1 = 0, int a2 = 360)
{
    fl_color(color);
    fl_pie(x - r, y - r, 2 * r, 2 * r, a1, a2);
}
int sign(double a)
{
    if (a >= 0)
    {
        return 1;
    }
    return -1;
}

void initialise(int R, int an = 0)
{
    double t = (double)GAP / (R * 2 * a_const);
    circle(Window_W / 2, Window_H / 2, R, 2, t + an, 360 - t + an);
    circle(Window_W / 2, Window_H / 2, R - thikness, 0, t + an, 360 - t + an);
}
class Ball : public Fl_Widget
{
    int angle, radius, R;

public:
    Ball(double R = R0, double r = r_ball, double a = 0) : Fl_Widget(0, 0, Window_W, Window_H)
    {
        angle = a;
        radius = r;
        this->R = R - GAP / 2;
    }

    void draw()
    {
        circle(Window_W / 2 + R * cos(angle * a_const), Window_H / 2 - R * sin(angle * a_const), radius, color_b);
    }
    void wipe()
    {
        circle(Window_W / 2 + R * cos(angle * a_const), Window_H / 2 - R * sin(angle * a_const), radius, 0);
    }
    void change_R(int r)
    {
        R = r - GAP / 2;
    }
    void move(int ag)
    {
        int d = diff(angle , ag);
        if (d > 170)
        {
            angle -= 10;
        }

        angle = angle + (int)(10 * sin((ag - angle) * a_const));
    }

    bool check(int a, int temp)
    {
        if (angle < 0)
        {
            angle += 360;
        }
        if (temp < 0)
        {
            temp += 360;
        }

        if ((angle >= a && temp <= a) || (angle <= a && temp >= a))
        {
            return true;
        }

        cout << temp << " " << angle << " " << a << endl;

        return false;
    }
    void refresh(int ag)
    {
        wipe();
        if (angle < 0)
        {
            angle += 360;
        }
        angle = angle % 360;
        move(ag);
        if ((cos((angle - ag) * a_const)) > cos(a_const * 75))
        {
            if (getr() == R3 && abs(angle - 240) < 20)
            {
                change_R(R2);
            }
            else if (getr() == R2 && abs(angle - 120) < 20)
            {
                change_R(R1);
            }
            else if (getr() == R1 && abs(angle - 360) < 20)
            {
                change_R(R0);
            }
        }
        if ((cos((angle - ag) * a_const)) < -1 * cos(a_const * 75))
        {
            if (getr() == R2 && abs(angle - 240) < 20)
            {
                change_R(R3);
            }
            else if (getr() == R1 && abs(angle - 120) < 20)
            {
                change_R(R2);
            }
            else if (getr() == R0 && abs(angle - 360) < 20)
            {
                change_R(R1);
            }
        }
        angle = angle % 360;
        redraw();
    }
    int getr()
    {
        return R + GAP / 2;
    }
    int geta(int a = 0)
    {
        return angle - a;
    }
    void seta(int a)
    {
        angle = a;
    }
};

class Game : public Fl_Widget
{
    Ball b[3];
    int ag;

public:
    Game() : Fl_Widget(0, 0, Window_W, Window_H)
    {
        b[0].seta(310);
        b[1].seta(160);
        b[2].seta(100);
        ag = 270;
    }
    int gap(int i)
    {
        if (b[i].getr() == R0)
        {
            return 15;
        }
        if (b[i].getr() == R1)
        {
            return 20;
        }
        if (b[i].getr() == R2)
        {
            return 30;
        }
        return 35;
    }
    void initial()
    {
        initialise(R0, 0);
        initialise(R0, 180);
        initialise(R1, 0);
        initialise(R2, 120);
        initialise(R3, 240);
    }
    void draw()
    {
        initial();
        b[0].refresh(ag);
        b[1].refresh(ag + gap(1));
        b[2].refresh(ag - gap(2));
    }
    int handle(int e)
    {
        if (e == FL_PUSH)
        {
            double x = Fl::event_x() - Window_W / 2;
            double y = Window_H / 2 - Fl::event_y();
            double z = sqrt(x * x + y * y);
            arrow(x / z, y / z);
            if (y >= 0)
            {
                ag = acos(x / z) / a_const;
            }
            else
            {
                ag = 360 - (acos(x / z) / a_const);
            }
        }

        return 0;
    }
    void arrow(double an, double bn)
    {
        static double a = 0, b = 0;
        fl_color(0);
        fl_line(Window_W / 2 + R0 * a, Window_H / 2 - R0 * b, Window_W / 2 + (R0 + GAP) * a, Window_H / 2 - (R0 + GAP) * b);
        a = an;
        b = bn;
        fl_color(color_line);
        fl_line(Window_W / 2 + R0 * a, Window_H / 2 - R0 * b, Window_W / 2 + (R0 + GAP) * a, Window_H / 2 - (R0 + GAP) * b);
    }
    void refresh()
    {
        redraw();
    }
    bool check()
    {
        if (b[0].getr() == R3 && b[1].getr() == R3 && b[2].getr() == R3)
        {
            fl_color(color_end);
            fl_font(FL_COURIER, 50);
            fl_draw("!! CONGRATULATION !!", Window_W / 4, Window_H / 9);
            return true;
        }
        return false;
    }
};

void change(void *a)
{
    if (((Game *)a)->check())
    {
        return;
    }
    ((Game *)a)->refresh();
    Fl::repeat_timeout(.04, change, a);
}

int main()
{
    srand(time(0));
    Fl_Window w(Window_W, Window_H, "GAME");
    w.color(0);
    Game G;
    Fl::repeat_timeout(.2, change, &G);
    w.show();
    return Fl::run();
}
