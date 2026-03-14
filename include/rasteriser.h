#ifndef RASTERISER_H_
#define RASTERISER_H_

#include <stdint.h>

typedef struct RenderPoint {
    int x, y;
} RenderPoint;

RenderPoint render_point_init(int x, int y);

typedef struct RenderTriangle {
    RenderPoint v1, v2, v3;
    int area;
} RenderTriangle;

RenderTriangle render_triangle_init(RenderPoint v1, RenderPoint v2, RenderPoint v3);
void render_triangle_draw(RenderTriangle * self, int width, int height, uint32_t buffer);

#endif