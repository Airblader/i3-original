/*
 * vim:ts=4:sw=4:expandtab
 *
 * i3 - an improved dynamic tiling window manager
 * © Michael Stapelberg and contributors (see also: LICENSE)
 *
 */
#include <stdlib.h>
#include <stdint.h>

#include <i3_cairo.h>
#include <libi3.h>

extern xcb_connection_t *conn;
extern xcb_screen_t *root_screen;

/*
 * Polyfill for xcb_poly_line using cairo to draw the line.
 *
 */
void i3_cairo_poly_line(xcb_connection_t *conn, xcb_drawable_t drawable, uint32_t points_len, const xcb_point_t *points, i3_cairo_line_opts gc) {
    // TODO correct width and height
    cairo_surface_t *surface = cairo_xcb_surface_create(conn, drawable, get_visualtype(root_screen), root_screen->width_in_pixels, root_screen->height_in_pixels);
    cairo_t *cr = cairo_create(surface);

    // TODO correct color
    cairo_set_source_rgb(cr, 1.0, 0.0, 0.0);
    cairo_set_line_width(cr, gc.line_width);

    for (int i = 0; i < points_len; i++) {
        cairo_line_to(cr, points[i].x, points[i].y);
    }

    cairo_stroke(cr);

    cairo_destroy(cr);
    cairo_surface_destroy(surface);
}
