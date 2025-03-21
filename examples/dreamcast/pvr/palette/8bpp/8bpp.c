/* KallistiOS ##version##

   8bpp.c
   Copyright (C) 2011 Tvspelsfreak
   Copyright (C) 2024 Andress Barajas
*/

/*
   8bpp example written by Tvspelsfreak

   This demo showcases a simple use of 8-bit palette-based textures on the 
   Dreamcast. The demo generates a radial gradient texture in RAM and uploads 
   it to VRAM. The gradient texture is rendered as a static image, but the colors
   in the palette are continuously cycled through a smooth transition effect, 
   making the gradient appear to animate over time.

   The palette animation is driven by a simple sine wave, with the red, green, 
   and blue channels modulating based on the frame number. The player can press 
   the START button to exit the demo.
*/

#include <stdlib.h>
#include <math.h>

#include <dc/pvr.h>
#include <dc/maple.h>
#include <dc/fmath.h>
#include <dc/maple/controller.h>

#define TEXTURE_WIDTH 256
#define TEXTURE_HEIGHT 256
#define PALETTE_ENTRY_COUNT 256

static pvr_poly_hdr_t hdr;

static void draw_screen(void) {
    pvr_vertex_t vert;

    pvr_prim(&hdr, sizeof(hdr));

    vert.argb = PVR_PACK_COLOR(1.0f, 1.0f, 1.0f, 1.0f);
    vert.oargb = 0;
    vert.flags = PVR_CMD_VERTEX;

    vert.x = 0.0f;
    vert.y = 0.0f;
    vert.z = 1.0f;
    vert.u = 0.0f;
    vert.v = 0.0f;
    pvr_prim(&vert, sizeof(vert));

    vert.x = 640.0f;
    vert.y = 0.0f;
    vert.z = 1.0f;
    vert.u = 1.0f;
    vert.v = 0.0f;
    pvr_prim(&vert, sizeof(vert));

    vert.x = 0.0f;
    vert.y = 480.0f;
    vert.z = 1.0f;
    vert.u = 0.0f;
    vert.v = 1.0f;
    pvr_prim(&vert, sizeof(vert));

    vert.x = 640.0f;
    vert.y = 480.0f;
    vert.z = 1.0f;
    vert.u = 1.0f;
    vert.v = 1.0f;
    vert.flags = PVR_CMD_VERTEX_EOL;
    pvr_prim(&vert, sizeof(vert));
}

static float distance(float x0, float y0, float x1, float y1) {
    const float dx = x1 - x0;
    const float dy = y1 - y0;

    return fsqrt(dx * dx + dy * dy);
}

static pvr_ptr_t generate_texture(uint32_t width, uint32_t height) {
    int i, j, mid_x, mid_y;
    float max_dist;

    uint8_t *texbuf;
    pvr_ptr_t texptr;

    /* Calculate texture midpoint and greatest distance from the
       midpoint to another point for future usage */

    mid_x = width / 2;
    mid_y = height / 2;
    max_dist = distance(0, 0, mid_x, mid_y);

    /* Allocate temp storage in RAM to generate texture in */
    texbuf = calloc(width * height, sizeof(uint8_t));

    /* Generate the texture */
    for(i = 0; i < height; i++)
        for(j = 0; j < width; j++)
            texbuf[i * width + j] = fsin((distance(j, i, mid_x, mid_y) / max_dist) * F_PI) * 256.0f;

    /* Allocate VRAM for the texture and upload it, twiddling it in the process */
    texptr = pvr_mem_malloc(width * height);
    pvr_txr_load_ex(texbuf, texptr, width, height, PVR_TXRLOAD_8BPP);

    /* As the texture is now residing in VRAM, we can free the temp storage
       and return the VRAM pointer */
    free(texbuf);

    return texptr;
}

static void animate_palette(uint32_t frame) {
    uint32_t i, val;

    for(i = 0;i < PALETTE_ENTRY_COUNT; i++) {
        val = (frame + i) & 0xFF;
        pvr_set_pal_entry(i, 0xFF00003F | ( val << 16 ) | ( val/2 << 8 ));
    }
}

static int check_start(void) {
    MAPLE_FOREACH_BEGIN(MAPLE_FUNC_CONTROLLER, cont_state_t, st)

    if(st->buttons & CONT_START)
        return 1;

    MAPLE_FOREACH_END()
    return 0;
}

int main(int argc, char** argv) {
    uint32_t frame;
    pvr_ptr_t texptr;
    pvr_poly_cxt_t cxt;

    pvr_init_defaults();

    /* First select a palette format */
    pvr_set_pal_format(PVR_PAL_ARGB8888);

    /* Initialize the texture */
    texptr = generate_texture(TEXTURE_WIDTH, TEXTURE_HEIGHT);

    /* Setup PVR context */
    pvr_poly_cxt_txr(&cxt, PVR_LIST_OP_POLY, PVR_TXRFMT_PAL8BPP | 
                    PVR_TXRFMT_8BPP_PAL(0), TEXTURE_WIDTH, TEXTURE_HEIGHT, 
                    texptr, PVR_FILTER_BILINEAR);
    pvr_poly_compile(&hdr, &cxt);

    frame = 0;
    while(!check_start()) {
        frame = (frame + 1) % 256;

        animate_palette(frame);

        pvr_wait_ready();
        pvr_scene_begin();

        pvr_list_begin(PVR_LIST_OP_POLY);

        draw_screen();

        pvr_list_finish();
        pvr_scene_finish();
    }

    pvr_mem_free(texptr);

    return 0;
}
