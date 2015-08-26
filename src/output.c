#undef I3__FILE__
#define I3__FILE__ "output.c"
/*
 * vim:ts=4:sw=4:expandtab
 *
 * i3 - an improved dynamic tiling window manager
 * © 2009 Michael Stapelberg and contributors (see also: LICENSE)
 *
 * output.c: Output (monitor) related functions.
 *
 */
#include "all.h"

/*
 * Returns the output container below the given output container.
 *
 */
Con *output_get_content(Con *output) {
    Con *child;

    TAILQ_FOREACH(child, &(output->nodes_head), nodes)
    if (child->type == CT_CON)
        return child;

    return NULL;
}

/*
 * Returns an 'output' corresponding to one of left/right/down/up or a specific
 * output name.
 *
 */
Output *get_output_from_string(Output *current_output, const char *output_str) {
    Output *output;

    if (strcasecmp(output_str, "left") == 0)
        output = get_output_next_wrap(D_LEFT, current_output);
    else if (strcasecmp(output_str, "right") == 0)
        output = get_output_next_wrap(D_RIGHT, current_output);
    else if (strcasecmp(output_str, "up") == 0)
        output = get_output_next_wrap(D_UP, current_output);
    else if (strcasecmp(output_str, "down") == 0)
        output = get_output_next_wrap(D_DOWN, current_output);
    else
        output = get_output_by_name(output_str);

    return output;
}

/*
 * Iterates over all outputs and pushes sticky windows to the currently visible
 * workspace on that output.
 *
 */
void output_push_sticky_windows(void) {
    Con *output;
    TAILQ_FOREACH(output, &(croot->nodes_head), nodes) {
        /* We cannot simply iterate over the floating windows and move them as
         * this will modify the list concurrently. Instead, we first figure out
         * how many sticky windows there are, then which ones and then we move them. */
        Con *workspace, *visible_ws;
        int num_sticky = 0;
        TAILQ_FOREACH(workspace, &(output_get_content(output)->nodes_head), nodes) {
            if (workspace_is_visible(workspace)) {
                visible_ws = workspace;
                /* We don't need to care for sticky windows on the visible workspace. */
                continue;
            }

            Con *child;
            TAILQ_FOREACH(child, &(workspace->floating_head), floating_windows) {
                if (con_is_sticky(child))
                    num_sticky++;
            }
        }

        Con *sticky_cons[num_sticky];
        int ctr = 0;
        TAILQ_FOREACH(workspace, &(output_get_content(output)->nodes_head), nodes) {
            if (workspace_is_visible(workspace))
                continue;

            Con *child;
            TAILQ_FOREACH(child, &(workspace->floating_head), floating_windows) {
                if (con_is_sticky(child))
                    sticky_cons[ctr++] = child;
            }
        }

        /* Now we can actually push the sticky windows to the front. */
        for (int i = 0; i < num_sticky; i++) {
            con_move_to_workspace(sticky_cons[i], visible_ws, true, false, true);
        }
    }
}
