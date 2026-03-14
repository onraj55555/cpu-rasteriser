#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>

#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;

typedef struct RenderPoint {
    int x, y;
} RenderPoint;

RenderPoint render_point_init(int x, int y) {
    return (RenderPoint) {.x = x, .y = y};
}

typedef struct RenderTriangle {
    RenderPoint v1, v2, v3;
} RenderTriangle;

static RenderTriangle rt = { 0 };

RenderTriangle render_triangle_init(RenderPoint v1, RenderPoint v2, RenderPoint v3) {
    return (RenderTriangle) {.v1 = v1, .v2 = v2, .v3 = v3};
}

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

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])
{
    SDL_SetAppMetadata("Example Renderer Clear", "1.0", "com.example.renderer-clear");

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    if (!SDL_CreateWindowAndRenderer("examples/renderer/clear", 640, 480, SDL_WINDOW_RESIZABLE, &window, &renderer)) {
        SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }
    SDL_SetRenderLogicalPresentation(renderer, 640, 480, SDL_LOGICAL_PRESENTATION_LETTERBOX);

    rt = render_triangle_init(render_point_init(100, 100), render_point_init(100, 200), render_point_init(200, 200));

    return SDL_APP_CONTINUE;  /* carry on with the program! */
}

/* This function runs when a new event (mouse input, keypresses, etc) occurs. */
SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
    if (event->type == SDL_EVENT_QUIT) {
        return SDL_APP_SUCCESS;  /* end the program, reporting success to the OS. */
    }
    return SDL_APP_CONTINUE;  /* carry on with the program! */
}

/* This function runs once per frame, and is the heart of the program. */
SDL_AppResult SDL_AppIterate(void *appstate)
{
    const double now = ((double)SDL_GetTicks()) / 1000.0;  /* convert from milliseconds to seconds. */
    /* choose the color for the frame we will draw. The sine wave trick makes it fade between colors smoothly. */
    const float red = (float) (0.5 + 0.5 * SDL_sin(now));
    const float green = (float) (0.5 + 0.5 * SDL_sin(now + SDL_PI_D * 2 / 3));
    const float blue = (float) (0.5 + 0.5 * SDL_sin(now + SDL_PI_D * 4 / 3));
    SDL_SetRenderDrawColorFloat(renderer, red, green, blue, SDL_ALPHA_OPAQUE_FLOAT);  /* new color, full alpha. */

    /* clear the window to the draw color. */
    SDL_RenderClear(renderer);

    SDL_Texture * buffer = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, 640, 480);

    render_triangle_draw(&rt, buffer);

    SDL_RenderTexture(renderer, buffer, NULL, NULL);

    /* put the newly-cleared rendering on the screen. */
    SDL_RenderPresent(renderer);

    return SDL_APP_CONTINUE;  /* carry on with the program! */
}

/* This function runs once at shutdown. */
void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
    /* SDL will clean up the window/renderer for us. */
}