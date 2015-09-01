/*
 * vim:ts=4:sw=4:expandtab
 *
 * i3 - an improved dynamic tiling window manager
 * © Michael Stapelberg and contributors (see also: LICENSE)
 *
 * i3_cairo: utility functions to replace calls to the XCB drawing API with
 * cairo.
 *
 */
#pragma once

#include <xcb/xcb.h>
#include <cairo/cairo-xcb.h>

/*
 * TODO
 *  - xcb_poly_line
 *      - foreground, background, line width
 *  - xcb_poly_fill_rectangle
 *      - foreground, background
 *  - xcb_copy_area (?)
 */

typedef struct i3_cairo_line_opts {
    uint32_t foreground;
    uint32_t background;
    uint32_t line_width;
} i3_cairo_line_opts;

/**
 * Polyfill for xcb_poly_line using cairo to draw the line.
 *
 */
void i3_cairo_poly_line(xcb_connection_t *conn, xcb_drawable_t drawable, uint32_t points_len, const xcb_point_t *points, i3_cairo_line_opts gc);
