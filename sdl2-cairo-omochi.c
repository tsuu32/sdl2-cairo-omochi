#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <cairo/cairo.h>

void
draw_and_update_texture(SDL_Texture *texture, int width, int height, int window_width, int window_height)
{
  void *pixels;
  int pitch;
  SDL_LockTexture(texture, NULL, &pixels, &pitch);
  
  cairo_surface_t *cr_surface = cairo_image_surface_create_for_data((unsigned char *)pixels,
                                                                    CAIRO_FORMAT_ARGB32,
                                                                    width,
                                                                    height,
                                                                    pitch);

  cairo_surface_set_device_scale(cr_surface, width / window_width, height / window_height);

  cairo_t *cr = cairo_create(cr_surface);

  // White background with cairo API
  cairo_set_source_rgba(cr, 1, 1, 1, 1.0);
  cairo_rectangle(cr, 0, 0, 640, 480);
  cairo_fill(cr);

  static float ay = 0.025;
  static float vy = 0;
  static float y = 0;
  static float ax = 0;
  static float vx = 4;
  static float x = 0;

  // omochi?
  cairo_move_to(cr, 50+x, 28+y);
  cairo_curve_to(cr, 64+x, 28+y, 76+x, 35+y, 76+x, 45+y);
  cairo_curve_to(cr, 76+x, 49+y, 72+x, 53+y, 67+x, 56+y);
  cairo_curve_to(cr, 79+x, 59+y, 84+x, 68+y, 84+x, 75+y);
  cairo_curve_to(cr, 84+x, 87+y, 75+x, 92+y, 68+x, 92+y);
  cairo_line_to(cr, 32+x, 92+y);
  cairo_curve_to(cr, 25+x, 92+y, 16+x, 87+y, 16+x, 75+y);
  cairo_curve_to(cr, 16+x, 68+y, 21+x, 59+y, 33+x, 56+y);
  cairo_curve_to(cr, 28+x, 53+y, 24+x, 49+y, 24+x, 45+y);
  cairo_curve_to(cr, 24+x, 35+y, 36+x, 28+y, 50+x, 28+y);
  cairo_close_path(cr);  

  cairo_set_source_rgba(cr, 1 - y/380, 1, 1 - y/380, 1.0);
  cairo_fill_preserve(cr);

  cairo_set_source_rgba(cr, 0, 0, 0, 1.0);
  cairo_set_line_width(cr, 2.0);
  cairo_set_line_cap(cr, CAIRO_LINE_CAP_ROUND);
  cairo_set_line_join(cr, CAIRO_LINE_JOIN_ROUND);
  cairo_stroke(cr);

  cairo_destroy(cr);
  cairo_surface_destroy(cr_surface);

  SDL_UnlockTexture(texture);

  vy += ay;
  y += vy;
  if (y > 380) {
    vy = -vy;
    y = -y+2*380;
  }
  
  vx += ax;
  x += vx;
  if (x > 540 || x < 0) {
    vx = -vx;
    x = (x > 540)
      ? -x+2*540
      : -x;
  }
}

int
main(int argc, char *argv[])
{
  SDL_Init(SDL_INIT_VIDEO);

  SDL_Window *window = SDL_CreateWindow("⛄️",
                                        SDL_WINDOWPOS_UNDEFINED,
                                        SDL_WINDOWPOS_UNDEFINED,
                                        640,
                                        480,
                                        SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI);

  SDL_Renderer *renderer = SDL_CreateRenderer(window,
                                              -1,
                                              SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

  int window_width;
  int window_height;
  SDL_GetWindowSize(window, &window_width, &window_height);

  int renderer_width;
  int renderer_height;
  SDL_GetRendererOutputSize(renderer, &renderer_width, &renderer_height);

  SDL_Texture *texture = SDL_CreateTexture(renderer,
                                           SDL_PIXELFORMAT_ARGB8888,
                                           SDL_TEXTUREACCESS_STREAMING,
                                           renderer_width,
                                           renderer_height);

  bool done = false;
  while (!done) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      switch (event.type) {
      case SDL_KEYDOWN:
        switch (event.key.keysym.sym) {
        case SDLK_q:
          done = true;
          break;
        default:
          break;
        }
        break;
      case SDL_QUIT:
        done = true;
        break;
      default:
        break;
      }
    }
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    draw_and_update_texture(texture, renderer_width, renderer_height, window_width, window_height);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);

    SDL_Delay(10);
  }

  SDL_DestroyTexture(texture);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);

  SDL_Quit();

  return 0;
}

// Local Variables:
// c-basic-offset: 2
// End:
