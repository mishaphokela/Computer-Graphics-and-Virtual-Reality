#define GL_SILENCE_DEPRECATION

#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>

// Convert pixel coordinates to OpenGL coordinates
float convertX(int x)
{
    return (float)x / 400.0f - 1.0f;
}

float convertY(int y)
{
    return (float)y / 300.0f - 1.0f;
}

// ---------------- BRESENHAM LINE ----------------

void drawLine(int x1, int y1, int x2, int y2)
{
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);

    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;

    int error = dx - dy;

    glBegin(GL_POINTS);

    while (true)
    {
        glVertex2f(convertX(x1), convertY(y1));

        if (x1 == x2 && y1 == y2)
            break;

        int e2 = 2 * error;

        if (e2 > -dy)
        {
            error -= dy;
            x1 += sx;
        }

        if (e2 < dx)
        {
            error += dx;
            y1 += sy;
        }
    }

    glEnd();
}

// ---------------- BRESENHAM CIRCLE ----------------

void plotCirclePoints(int xc, int yc, int x, int y)
{
    glVertex2f(convertX(xc + x), convertY(yc + y));
    glVertex2f(convertX(xc - x), convertY(yc + y));
    glVertex2f(convertX(xc + x), convertY(yc - y));
    glVertex2f(convertX(xc - x), convertY(yc - y));

    glVertex2f(convertX(xc + y), convertY(yc + x));
    glVertex2f(convertX(xc - y), convertY(yc + x));
    glVertex2f(convertX(xc + y), convertY(yc - x));
    glVertex2f(convertX(xc - y), convertY(yc - x));
}

void drawCircle(int xc, int yc, int radius)
{
    int x = 0;
    int y = radius;

    int decision = 3 - 2 * radius;

    glBegin(GL_POINTS);

    while (x <= y)
    {
        plotCirclePoints(xc, yc, x, y);

        if (decision < 0)
        {
            decision = decision + 4 * x + 6;
        }
        else
        {
            decision = decision + 4 * (x - y) + 10;
            y--;
        }

        x++;
    }

    glEnd();
}

// ---------------- MAIN ----------------

int main()
{
    if (!glfwInit())
    {
        std::cout << "GLFW initialization failed!" << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

    GLFWwindow* window = glfwCreateWindow(
        800,
        600,
        "Bresenham Line and Circle",
        nullptr,
        nullptr
    );

    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    glPointSize(3.0f);

    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Draw line
        glColor3f(0.2f, 0.8f, 1.0f);
        drawLine(50, 100, 350, 500);

        // Draw circle
        glColor3f(1.0f, 0.4f, 0.2f);
        drawCircle(600, 300, 150);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
