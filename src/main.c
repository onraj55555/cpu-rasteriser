#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>







static RenderTriangle rt = { 0 };



int sign(RenderPoint p1, RenderPoint p2, RenderPoint p3) {
    return (p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p1.y - p3.y);
}

int render_triangle_render_point_capture(RenderTriangle * self, RenderPoint pt) {
    int d1, d2, d3;
    int has_neg, has_pos;

    d1 = sign(pt, self->v1, self->v2);
    d2 = sign(pt, self->v2, self->v3);
    d3 = sign(pt, self->v3, self->v1);

    has_neg = (d1 < 0) || (d2 < 0) || (d3 < 0);
    has_pos = (d1 > 0) || (d2 > 0) || (d3 > 0);

    return !(has_neg && has_pos);
}

void draw_horizontal_line(int x0, int x1, int y, uint32_t * pixels) {
    for(int i = x0; i <= x1; i++) {
        pixels[y * 640 + i] = 0xFFFFFFFF;
    }
}


void render_triangle_draw(RenderTriangle * self, SDL_Texture * buffer) {
    int x0 = fminf(fminf(self->v1.x, self->v2.x), self->v3.x);
    int y0 = fminf(fminf(self->v1.y, self->v2.y), self->v3.y);
    int x1 = fmaxf(fmaxf(self->v1.x, self->v2.x), self->v3.x);
    int y1 = fmaxf(fmaxf(self->v1.y, self->v2.y), self->v3.y);

    int * pixels;
    int pitch;
    SDL_LockTexture(buffer, NULL, (void **)&pixels, &pitch);

    int colour = 0xFFFFFFFF;

    //printf("pitch = %d\n", pitch);
    //exit(EXIT_FAILURE);

    for(int y = y0; y <= y1; y++) {
        for(int x = x0; x <= x1; x++) {
            if(render_triangle_render_point_capture(self, (RenderPoint){.x = x, .y = y})) {
                pixels[y * 640 + x] = colour;
            }
        }
    }

    SDL_UnlockTexture(buffer);
}