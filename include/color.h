#ifndef COLOR_H
#define COLOR_H

typedef struct
{
    float r;
    float g;
    float b;
    //alpha is not needed
} Color;

static Color ColorRed()
{
    Color c;
    c.r = 1.f;
    c.g = 0.f;
    c.b = 0.f;
    return c;
}


static Color ColorBlack()
{
    Color c;
    c.r = 0.f;
    c.g = 0.f;
    c.b = 0.f;
    return c;
}

static Color MultiplyColor(Color* color, float s)
{
    Color r;
    r.r = color->r * s;
    r.g = color->g * s;
    r.b = color->b * s;
    return r;
}

#endif