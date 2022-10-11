// l031.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <iomanip>  // set precision
#include <climits>  // int_max
#include <list>     // list
#include <math.h>

const int size = 800;

// point class
// private, getters, setters, constructors
class Point {
    public:
        void setX(double x);
        double getX(void);
        void setY(double y);
        double getY(void);
        bool diff(Point p1, Point p2);
        Point(double x, double y);  // This is the constructor
        Point(void);
    private:
        double x;
        double y;
};

Point::Point(void) {
    x = 0;
    y = 0;
}

Point::Point(double xval, double yval) {
    x = xval;
    y = yval;
}

void Point::setX(double xval) {
    x = xval;
}
double Point::getX(void) {
    return x;
}
void Point::setY(double yval) {
    y = yval;
}
double Point::getY(void) {
    return y;
}
bool diff(Point p1, Point p2) {
    if (p1.getX() != p2.getX() && p1.getY() != p2.getY())
        return true;
    return false;
}

void illuminate(int** arr, int y, int x, int color) {
    if (color == 0)
        arr[x][y] = 0;
    else if (color == 1)
        arr[x][y] = 1;
    else if (color == 2)
        arr[x][y] = 2;
}

void notoutofbounds(int** array, int x, int y, int color) {
    if (x >= 0 && x < size && y >= 0 && y < size)
        //array[x][y] = 0;
        illuminate(array, x, y, color);
}

double distance(double x1, double y1, double x2, double y2) {
    double t1 = pow(x1 - x2, 2);
    double t2 = pow(y1 - y2, 2);
    //std::cout << t1 << " " << t2 << " " << sqrt(t1 + t2) << "\n";
    return (double)(sqrt(t1 + t2));
}

void circle(int** array, int xcenter, int ycenter, int R, int color) {
    int y = R;
    int xmax = (int)(R * 0.70710678);
    int ysquare = y * y;
    int ty = (2 * y) - 1;
    int y2_new = ysquare;

    //std::cout << y << " " << xmax << " " << ysquare << " " << ty << " " << y2_new << "\n";

    for (int x = 0; x <= xmax + 1; x++) {
        if ((ysquare - y2_new) >= ty) {
            ysquare -= ty;
            y -= 1;
            ty -= 2;
        }
        notoutofbounds(array, y + xcenter, x + ycenter, color);
        notoutofbounds(array, -y + xcenter, x + ycenter, color);
        notoutofbounds(array, y + xcenter, -x + ycenter, color);
        notoutofbounds(array, -y + xcenter, -x + ycenter, color);

        notoutofbounds(array, x + xcenter, y + ycenter, color);
        notoutofbounds(array, -x + xcenter, y + ycenter, color);
        notoutofbounds(array, x + xcenter, -y + ycenter, color);
        notoutofbounds(array, -x + xcenter, -y + ycenter, color);

        y2_new -= (2 * x) - 3;
    }
}

void part1() {
    std::ofstream points;
    points.open("points.txt");
    std::list <Point*> pointlist;

    // make array
    int** array;
    array = new int* [size];

    // fill with 1s
    for (int row = 0; row < size; ++row)
        array[row] = new int[size];
    for (int row = 0; row < size; ++row)
        for (int col = 0; col < size; ++col)
            array[row][col] = 1;

    srand(time(NULL));
    for (int i = 0; i <= 59; i++) {
        double x = double(rand()) / INT_MAX;
        double y = double(rand()) / INT_MAX;
        pointlist.push_back(new Point(x, y));
        circle(array, (int)(x * size), (int)(y * size), 3, 0);
        points << std::fixed;
        points << std::setprecision(23) << x << "  " << y << "\n";
    }
    points.close();

    // make ppm
    std::ofstream ppm;
    ppm.open("points.ppm");
    ppm << "P3 " << size << " " << size << " 255\n";

    // iterator
    double d = INT_MAX;
    Point dp1;
    Point dp2;
    Point temp1;
    Point temp2;
    std::list <Point*> ::iterator it1;
    std::list <Point*> ::iterator it2;
    for (it1 = pointlist.begin(); it1 != pointlist.end(); it1++) {
        for (it2 = it1++; it2 != pointlist.end(); it2++) {
            temp1 = **it1;
            temp2 = **it2;
            if (diff(temp1, temp2)) {                // std::cout << temp1.getX() << " " << temp1.getY() << " \t" << temp2.getX() << " " << temp2.getY() << "\n";
                double dist = distance(temp1.getX(), temp1.getY(), temp2.getX(), temp2.getY());
                // std::cout << dist << "\n";
                if (dist < d) {
                    d = dist;
                    dp1 = temp1;
                    dp2 = temp2;
                }
            }
        }
    }

    // std::cout << "\n" << d << "\n";
    // std::cout << dp1.getX() << " " << dp1.getY() << "\n";
    // std::cout << dp2.getX() << " " << dp2.getY() << "\n";

    circle(array, (int)(dp1.getX() * size), (int)(dp1.getY() * size), 2, 2);
    circle(array, (int)(dp1.getX() * size), (int)(dp1.getY() * size), 3, 2);
    circle(array, (int)(dp2.getX() * size), (int)(dp2.getY() * size), 2, 2);
    circle(array, (int)(dp2.getX() * size), (int)(dp2.getY() * size), 3, 2);


    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            if (array[i][j] == 0)
                ppm << array[i][j] << " " << array[i][j] << " " << array[i][j] << ' ';
            else if (array[i][j] == 2)
                ppm << 255 << " " << 0 << " " << 0 << " ";
            else
                ppm << 255 << " " << 255 << " " << 255 << ' ';
        }
        ppm << std::endl;
    }
    ppm << "\n";

    ppm.close();
}

int main() {
    part1();
}