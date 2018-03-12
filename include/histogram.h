#ifndef HISTOGRAM_H
#define HISTOGRAM_H

/*
    Simple drawing histogram in console

    Author: Michal Kukowski
    email: michalkukowski10@gmail.com

    LICENCE: GPL3.0
*/

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>


/*
    Draw histogram for int only

    PARAMS
    @IN x_values - X axis values
    @IN y_values - Y axis values
    @IN x_len - len(x_values)
    @IN y_len - len(y_values)
    @IN title - your title
    @IN x_legend - X Axis description
    @IN y_legend - Y Axis description
    @IN reset - reset terminal before draw ?

    RETURN
    0 iff succes
    Non-zero values iff failure
*/
int histogram_int_draw( const int64_t *x_values,
                        const int64_t *y_values,
                        size_t x_len,
                        size_t y_len,
                        const char *title,
                        const char *x_legend,
                        const char *y_legend,
                        bool reset);

#endif