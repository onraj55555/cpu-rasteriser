#include "rasteriser.h"
#include <stdlib.h>
#include "debug.h"

RenderPoint render_point_init(int x, int y) {
    return (RenderPoint) {.x = x, .y = y};
}

RenderPoint render_point_absolute_distance(RenderPoint rp1 ,RenderPoint rp2) {
    return render_point_init(abs(rp1.x - rp2.x), abs(rp1.y - rp2.y));
}

RenderTriangle render_triangle_init(RenderPoint v1, RenderPoint v2, RenderPoint v3) {
    return (RenderTriangle) {.v1 = v1, .v2 = v2, .v3 = v3, .area = ((v2.x - v1.x) * (v3.y - v1.y) - (v3.x - v1.x) * (v2.y - v1.y)) / 2};
}

int area(RenderPoint p, RenderPoint a, RenderPoint b) {
    return ((a.x - p.x) * (b.y - p.y) - (b.x - p.x) * (a.y - p.y)) / 2;
}

void render_triangle_draw(RenderTriangle * self, int width, int height, uint32_t buffer) {
    // Split up triangle in upper and lower triangle
    /*int v_middle = -1;
    RenderPoint d12 = render_point_absolute_distance(self->v1, self->v2);
    RenderPoint d23 = render_point_absolute_distance(self->v2, self->v3);
    RenderPoint d13 = render_point_absolute_distance(self->v1, self->v3);

    // v3 is middle
    if(d12.y > d23.y && d12.y > d13.y) {
         v_middle = 3;
    }
    // v1 is middle
    else if(d23.y > d12.y && d23.y > d13.y) v_middle = 1;
    // v2 is middle
    else if(d13.y > d12.y && d13.y > d23.y) v_middle = 2;
    else PANIC("No middle vertex found");*/
}