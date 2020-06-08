#define SFML_STATIC

#include <math.h>
#include <stdio.h>

#include <SFML/Graphics.hpp>
#include <random>

#define GRADIENT_SIZE 10
#define PI 3.14159

using namespace sf;

float interpolate(float a0, float a1, float w) { return a0 + (a1 - a0) * w; }

void randomUnitVector(float vector[]) {
    float angle = (float)(rand() % 360) * PI / 180;
    vector[0] = sin(angle);
    vector[1] = cos(angle);
}

float dotGridGradient(float vectorGrid[][2], int gridSize, int gridX, int gridY, float x, float y) {
    float weightX = x - gridX;
    float weightY = y - gridY;
    return weightX * vectorGrid[gridY * gridSize + gridX][0] + weightY * vectorGrid[gridY * gridSize + gridX][1];
}

float fade(float x) { return x * x * x * (x * (x * 6 - 15) + 10); }

float perlin(float vectorGrid[][2], int gridSize, float x, float y) {
    int leftX = (int)x;
    int rightX = leftX + 1;
    int topY = (int)y;
    int bottomY = topY + 1;

    float u = fade(x - leftX);
    float v = fade(y - topY);

    float leftDot, rightDot;

    leftDot = dotGridGradient(vectorGrid, gridSize, leftX, topY, x, y);
    rightDot = dotGridGradient(vectorGrid, gridSize, rightX, topY, x, y);
    float topHorizontalInterpolate = interpolate(leftDot, rightDot, u);

    leftDot = dotGridGradient(vectorGrid, gridSize, leftX, bottomY, x, y);
    rightDot = dotGridGradient(vectorGrid, gridSize, rightX, bottomY, x, y);
    float bottomHorizontalInterpolate = interpolate(leftDot, rightDot, u);

    return interpolate(topHorizontalInterpolate, bottomHorizontalInterpolate, v);
}

int main(int argc, char* argv[]) {
    srand(time(NULL));

    const int size = 400;

    float vector[2];
    randomUnitVector(vector);

    int gridSize = 16;
    float vectorGrid[gridSize * gridSize][2];
    for (int y = 0; y < gridSize; y++) {
        for (int x = 0; x < gridSize; x++) {
            randomUnitVector(vectorGrid[y * gridSize + x]);
        }
    }

    RenderWindow window(VideoMode(size, size), "Perlin Noise");
    CircleShape shape(100.f);
    shape.setFillColor(Color::Green);

    Texture texture;
    if (!texture.create(size, size)) {
        printf("Failed to load texture!\n");
        return 1;
    }

    int index;
    float divisor = (float)size / (gridSize - 1);
    Color color[size * size];
    float noise, normalized;
    float r, g, b;
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            index = i * size + j;
            noise = perlin(vectorGrid, gridSize, j / divisor, i / divisor);
            noise += perlin(vectorGrid, gridSize, j / divisor / 4, i / divisor / 4);
            noise /= 2;
            normalized = (noise + 1.0f) / 2;
            r = g = b = normalized;
            color[index] = Color((int)(r * 0xFF), (int)(g * 0xFF), (int)(b * 0xFF));
        }
    }

    Uint8 pixels[size * size * 4];
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            index = i * size + j;
            pixels[index * 4 + 0] = color[index].r;
            pixels[index * 4 + 1] = color[index].g;
            pixels[index * 4 + 2] = color[index].b;
            pixels[index * 4 + 3] = color[index].a;
        }
    }

    texture.update(&pixels[0]);
    Sprite sprite(texture);

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) window.close();
        }
        window.clear();
        window.draw(shape);
        window.draw(sprite);
        window.display();
    }

    return 0;
}