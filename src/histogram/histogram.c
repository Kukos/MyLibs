#include <log.h>
#include <common.h>
#include <histogram.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <inttypes.h>
#include <math.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>

#define TITLE_STR "TITLE: "
#define X_AXIS_STR "X AXIS: "
#define Y_AXIS_STR "Y AXIS: "

#define LEGEND_LINES 5

#define X_AXIS_LINES 2
#define X_AXIS_COLUMNS 1

#define Y_AXIS_COLUMNS 1
#define Y_AXIS_LINES 2

#define reset_terminal() \
    do { \
        if (system("tput reset") == -1) \
            FATAL("Terminal reset failed\n"); \
    } while (0)

#define print_spaces(x) \
    do { \
        (void)printf("%*s", (int)(x), ""); \
    } while (0)

/*
    Get terminal size

    PARAMS
    @OUT w - terminal width
    @OUT h - terminal hight

    RETURN
    0 iff success
    Non-zero value iff failure
*/
static ___inline___ int get_terminal_size(size_t *w, size_t *h);

/*
    Write X axis to buffer if possible

    PARAMS
    @IN x_values - array with x axis values
    @IN x_len - len(x_values)
    @IN width - available width
    @OUT buffer_max_size - buffer max alloc size

    RETURN
    NULL iff failure
    Pointer to buffer iff success
*/
static ___inline___ char *write_x_to_buffer(const int64_t *x_values, size_t x_len, size_t width, size_t *buffer_max_size);

/*
    Get max from array

    PARAMS
    @IN t - array
    @IN len - len(t)

    RETURN
    MAX of array
*/
static ___inline___ int64_t max_from_array(const int64_t *t, size_t len);

/*
    Get max from array

    PARAMS
    @IN t - array
    @IN len - len(t)

    RETURN
    MIN of array
*/
static ___inline___ int64_t min_from_array(const int64_t *t, size_t len);

/*
    Resize X Axis

    PARAMS
    @IN buffer - buffer with X Axis
    @IN x_values - x values
    @IN buffer_size - max buffer size
    @IN len - x_values len

    RETURN
    Array with X axis element width iff success
    NULL iff failure
*/
static size_t *resize_x(char *buffer, const int64_t *x_values, size_t buffer_size, size_t len);

/*
    Draw legend of histogram

    PARAMS
    @IN title - title str
    @IN x - x axis str
    @IN y - y axis str
    @IN w - available width of terminal
    @IN h - available hight of terminal

    RETURN
    -1 iff failure
    No of consume new lines iff success
*/
static ___inline___ int draw_legend(const char *title, const char *x, const char *y, size_t w, size_t h);

/*
    Reserver width for Y Axis numbers

    PARAMS
    @IN y_values - array of y numbers
    @IN y_len - len(y_values)
    @IN width - available width

    RETURN
    -1 iff failure
    Num of reserved bytes
*/
static ___inline___ int reserve_width_for_y_numbers(const int64_t *y_values, size_t y_len, size_t width);

/*
    Draw X Axis

    PARAMS
    @IN buf - buf with x axis
    @IN offset - reserved offset

    RETURN
    THis is a void function
*/
static ___inline___ void draw_x_axis(const char *buf, size_t offset);

/*
    Draw Y axis

    PARAMS
    @IN values - Y Axis values
    @IN y_len - len(values)
    @IN height - available terminal height
    @IN offset - reserved offset
    @IN w_res - width reserved for x_values[i]

    RETURN
    0 iff success
    Non-zero value iff failure
*/
static int draw_y_axis(const int64_t *values, size_t y_len, size_t height, size_t offset, const size_t *w_res);

/*
    Get Value for 1 space for X Axis values

    PARAMS
    @IN x_values - X Axis values
    @IN x_len - len(x_values)
    @IN buf_cur_size - current size of buffer
    @IN buf_max_size - allocated buffer size

    RETURN
    Min Values for 1 space
*/
static ___inline___ double get_x_axis_value_for_space(const int64_t *x_values, size_t x_len, size_t buf_cur_size, size_t buf_max_size);

/*
    Get spaces for X Axis value

    PARAMS
    @IN x_values - X Axis values
    @IN i - i-th x_value
    @IN vs - value for space

    RETURN
    Spaces for i-th X Axis value
*/
static ___inline___ size_t get_spaces_for_x_value(const int64_t *x_values, size_t i, double vs);

/*
    Get Y Axis min value for char

    PARAMS
    @IN values - Y Axis values
    @IN len - len(values)
    @IN height - available terminal height

    RETURN
    Value for char
*/
static ___inline___ int64_t get_y_axis_value_for_c(const int64_t *values, size_t len, size_t height);

/*
    Draw single column

    PARAMS
    @IN values - copy of Y values
    @IN len - len(values)
    @IN offset - reserved offset for Y number
    @IN height - available  terminal height
    @IN vc - value for char
    @IN w_res - width reserved for X Value
    @IN / OUT line - printed line
    @IN / OUT val - value from print *
    @IN max - max(values)
    @IN min - min(values)

    RETURN
    This is a void function
*/
static void draw_single_column( int64_t *values,
                                size_t len,
                                size_t offset,
                                size_t height,
                                int64_t vc,
                                const size_t *w_res,
                                int64_t *line,
                                int64_t *val,
                                int64_t max,
                                int64_t min);

/*
    Reserve Width and Height for const histogram str

    PARAMS
    @IN / OUT width - available terminal width
    @IN / OUT height - available terminal height

    RETURN
    0 iff success
    Non-zero value iff failue
*/
static int reserve_w_h_for_const_str(size_t *width, size_t *height);

static ___inline___ int64_t max_from_array(const int64_t *t, size_t len)
{
    int64_t max;
    size_t i;

    TRACE();

    if (t == NULL)
        ERROR("t == NULL\n", INT64_MIN);

    if (len == 0)
        ERROR("len == 0\n", INT64_MIN);

    max = t[0];
    for (i = 1; i < len; ++i)
        max = MAX(max, t[i]);

    return max;
}

static ___inline___ int64_t min_from_array(const int64_t *t, size_t len)
{
    int64_t min;
    size_t i;

    TRACE();

    if (t == NULL)
        ERROR("t == NULL\n", INT64_MAX);

    if (len == 0)
        ERROR("len == 0\n", INT64_MAX);

    min = t[0];
    for (i = 1; i < len; ++i)
        min = MIN(min, t[i]);

    return min;
}

static ___inline___ int get_terminal_size(size_t *w, size_t *h)
{
    struct winsize ws;
    int ret;

    TRACE();

    if (w == NULL || h == NULL)
        ERROR("w == NULL || h == NULL\n", 1);

    ret = ioctl(0, TIOCGWINSZ, &ws);
    if (ret)
        ERROR("Ioctl error\n", ret);

    *w = (size_t)ws.ws_col;
    *h = (size_t)ws.ws_row;

    return 0;
}

static ___inline___ char *write_x_to_buffer(const int64_t *x_values, size_t x_len, size_t width, size_t *buf_max_size)
{
    char *buffer;
    char *ptr;
    size_t buffer_size;
    size_t len;
    size_t i;

    TRACE();

    if (width == 0)
        ERROR("width == 0\n", NULL);

    buffer_size = sizeof(char) * (width + 1);
    buffer = (char *)malloc(buffer_size);
    if (buffer == NULL)
        ERROR("Malloc error\n", NULL);

    *buf_max_size = buffer_size;

    /* simple writing */
    ptr = buffer;
    buffer[0] = '|';
    --buffer_size;
    ++ptr;
    for (i = 0; i < x_len; ++i)
    {
        len = (size_t)snprintf(ptr, buffer_size, "%" PRId64 "|", x_values[i]);
        LOG("FOR %" PRId64 " Len = %zu\n", x_values[i], len);
        if (len >= buffer_size)
        {
            FREE(buffer);
            ERROR("Cant create x axis\n", NULL);
        }

        ptr += len;
        buffer_size -= len;
    }

    return buffer;
}

static ___inline___ double get_x_axis_value_for_space(const int64_t *x_values, size_t x_len, size_t buf_cur_size, size_t buf_max_size)
{
    size_t buf_avail_size;
    int64_t max;
    int64_t min;

    double value_for_space;

    TRACE();

    buf_avail_size = buf_max_size - buf_cur_size - 1;
    LOG("BUF SIZE: cur = %zu, max = %zu, avai = %zu\n", buf_cur_size, buf_max_size, buf_avail_size);

    /* calc resizing parameters */
    max = max_from_array(x_values, x_len);
    if (max == INT64_MIN)
        ERROR("MAX error\n", 0.0);

    min = min_from_array(x_values, x_len);
    if (min == INT64_MAX)
        ERROR("MIN error\n", 0.0);

    if (min < 0)
        value_for_space = (double)(buf_avail_size - x_len) / (double)(max - min);
    else
        value_for_space = (double)(buf_avail_size) / (double)max;

    LOG("MIN = %" PRId64 " MAX = %" PRId64 " VS = %lf\n", min, max, value_for_space);

    return value_for_space;
}

static ___inline___ size_t get_spaces_for_x_value(const int64_t *x_values, size_t i, double vs)
{
    double spaces;
    uint64_t diff;

    TRACE();

    if (i == 0)
        diff = (uint64_t)0;
    else
        diff = (uint64_t)ABS(x_values[i] - x_values[i - 1]);

    spaces = floor((double)((double)diff * vs));

    return (size_t)spaces;
}

#define SKIP_NUMBER(ptr, len) \
    do { \
        while (*ptr != '\0' && isdigit(*ptr)) \
        { \
            ++ptr; \
            --len; \
        } \
    } while (0)

#define FIND_CHAR(ptr, c, len) \
    do { \
        while (*ptr != '\0' && *ptr != c) \
        { \
            ++ptr; \
            --len; \
        } \
        ++ptr; \
        --len; \
    } while (0)

#define INSERT_SPACES(ptr, to_insert, j, len) \
    do { \
        (void)memmove(ptr + to_insert, ptr, sizeof(char) * len); \
        for (j = 0; j < to_insert; ++j) \
        { \
            *ptr = ' '; \
            ++ptr; \
        } \
    } while (0)

static size_t *resize_x(char *buffer, const int64_t *x_values, size_t buffer_size, size_t len)
{
    size_t buf_cur_size;

    double value_for_space;

    size_t i;
    size_t j;
    size_t spaces;

    char *ptr;
    char *temp_ptr;

    size_t *w_val;

    TRACE();

    buf_cur_size = strlen(buffer);

    w_val = (size_t *)malloc(sizeof(size_t) * len);
    if (w_val == NULL)
        ERROR("Malloc error\n", NULL);

    value_for_space = get_x_axis_value_for_space(x_values, len, buf_cur_size, buffer_size);

    ptr = buffer;
    FIND_CHAR(ptr, '|', buf_cur_size);
    /* resize array */
    for (i = 0; i < len; ++i)
    {
        spaces = get_spaces_for_x_value(x_values, i, value_for_space);
        LOG("FOR %" PRId64 " spaces = %zu\n", x_values[i], spaces);
        if (spaces == 0)
        {
            /* find | */
            temp_ptr = ptr;
            SKIP_NUMBER(ptr, buf_cur_size);
            FIND_CHAR(ptr, '|', buf_cur_size);

            w_val[i] = (size_t)(ptr - temp_ptr - 1);
            LOG("w_val[%zu] = %zu\n", i, w_val[i]);
            continue;
        }

        /* find | */
        temp_ptr = ptr;

        /* insert first spaces */
        INSERT_SPACES(ptr, spaces, j, buf_cur_size);
        SKIP_NUMBER(ptr, buf_cur_size);
        FIND_CHAR(ptr, '|', buf_cur_size);

        w_val[i] = (size_t)(ptr - temp_ptr - 1);
        LOG("w_val[%zu] = %zu\n", i, w_val[i]);
    }
    ptr[1] = '\0';

    return w_val;
}

#undef FIND_CHAR
#undef SKIP_NUMBER
#undef INSERT_SPACES

static ___inline___ int draw_legend(const char *title, const char *x, const char *y, size_t w, size_t h)
{
    size_t len;
    size_t s;
    const size_t needed_lines = (size_t)LEGEND_LINES;

    TRACE();

    if (h < needed_lines)
        ERROR("To little height\n", -1);

    len = strlen(TITLE_STR);
    len += strlen(title);
    s = w - len - 1;

    print_spaces((s + 1) >> 1);
    printf(TITLE_STR "%s", title);
    print_spaces(s >> 1);

    printf("\n");
    printf(X_AXIS_STR "%s\n", x);
    printf(Y_AXIS_STR "%s\n", y);
    printf("\n\n");

    return (int)needed_lines; /* n of new lines */
}

static ___inline___ int reserve_width_for_y_numbers(const int64_t *y_values, size_t y_len, size_t width)
{
    int ret_max;
    int ret_min;

    TRACE();

    ret_max = snprintf(NULL, 0, "%" PRId64, max_from_array(y_values, y_len));
    if ((size_t)ret_max + Y_AXIS_COLUMNS >= width)
        ERROR("Cannot allocate space for Y Axis, width to low\n", -1);

    ret_min = snprintf(NULL, 0, "%" PRId64, min_from_array(y_values, y_len));
    if ((size_t)ret_min + Y_AXIS_COLUMNS >= width)
        ERROR("Cannot allocate space for Y Axis, width to low\n", -1);

    return MAX(ret_max, ret_min) + Y_AXIS_COLUMNS;
}

static ___inline___ void draw_x_axis(const char *buf, size_t offset)
{
    size_t i;
    size_t len;

    TRACE();

    len = strlen(buf);

    print_spaces(offset);
    for (i = 0; i < len; ++i)
        printf("-");

    printf(">\n");

    print_spaces(offset);
    printf("%s\n", buf);
}

static ___inline___ int64_t get_y_axis_value_for_c(const int64_t *values, size_t len, size_t height)
{
    int64_t min;
    int64_t max;
    int64_t val;
    int64_t range;

    TRACE();

    max = max_from_array(values, len);
    if (max == INT64_MIN)
        ERROR("MAX error\n", 0);

    min = min_from_array(values, len);
    if (min == INT64_MAX)
        ERROR("MIN error\n", 0);

    if (min < 0)
        range = (max - min);
    else
        range = max;

    val = (range + ((int64_t)height - 1)) / (int64_t)height;
    if (max - (val * (int64_t)(height - 1)) < 0 && min >= 0)
        --val;

    LOG("Height = %zu, MIN = %" PRId64 " MAX = %" PRId64 " value for Y Axis char = %" PRId64 "\n", height, min, max, val);

    return val;
}

static void draw_single_column( int64_t *values,
                                size_t len,
                                size_t offset,
                                size_t height,
                                int64_t vc,
                                const size_t *w_res,
                                int64_t *line,
                                int64_t *val,
                                int64_t max,
                                int64_t min)
{
    size_t i;

    TRACE();

    if (*line == 0)
        *val = max;
    else
        *val -= vc;

    if (*val == 0 && min >= 0)
        *val = 1;

    if ((size_t)*line == (height - 1) && min < 0)
    {
        LOG("The last line and min < 0\n");
        printf("%*" PRId64 "|", (int)offset, min);
        *val = min;
    }
    else
        printf("%*" PRId64 "|", (int)offset, *val);

    for (i = 0; i < len; ++i)
    {
        if (values[i] < *val)
            print_spaces(w_res[i]);
        else
        {
            print_spaces(w_res[i] - 1);
            printf("*");

            values[i] -= MIN(values[i], vc);
        }

        printf("|");
    }

    printf("\n");
    *line += 1;
}

static int draw_y_axis(const int64_t *values, size_t y_len, size_t height, size_t offset, const size_t *w_res)
{
    size_t i;
    int64_t value_for_c;
    int64_t *values_copy;
    int64_t min;
    int64_t max;
    int64_t line = 0;
    int64_t val = 0;

    TRACE();

    value_for_c = get_y_axis_value_for_c(values, y_len, height);
    values_copy = (int64_t *)malloc(sizeof(int64_t) * y_len);
    if (values_copy == NULL)
        ERROR("Malloc error\n", 1);

    (void)memcpy(values_copy, values, sizeof(int64_t) * y_len);

    min = min_from_array(values, y_len);
    max = max_from_array(values, y_len);

    /* print top line */
    print_spaces(offset);
    printf("^\n");

    for (i = 0; i < height; ++i)
        draw_single_column(values_copy, y_len, offset, height, value_for_c, w_res, &line, &val, max, min);

    FREE(values_copy);

    return 0;
}

static int reserve_w_h_for_const_str(size_t *width, size_t *height)
{
    TRACE();

    /* reserve lines for X_AXIS */
    if (*height < X_AXIS_LINES)
        ERROR("Cannot reserve lines for X AXIS\n", 1);

    *height -= X_AXIS_LINES;
    LOG("X AXIS Reserved %d lines\n", X_AXIS_LINES);

    /* reserve lines for  Y AXIS */
    if (*height < Y_AXIS_LINES)
        ERROR("Cannot reserve lines for Y AXIS\n", 1);

    *height -= Y_AXIS_LINES;
    LOG("Y AXIS Reserved %d lines\n", Y_AXIS_LINES);

    /* reserve columns for X Axis */
    if (*width < X_AXIS_COLUMNS)
        ERROR("Cannot reserve columns for X AXIS\n", 1);

    *width -= X_AXIS_COLUMNS;
    LOG("X Axis Reserved %d columns\n", X_AXIS_COLUMNS);

    return 0;
}

int histogram_int_draw( const int64_t *x_values,
                        const int64_t *y_values,
                        size_t x_len,
                        size_t y_len,
                        const char *title,
                        const char *x_legend,
                        const char *y_legend,
                        bool reset)
{
    size_t width;
    size_t height;

    char *buffer_x;
    size_t buffer_x_size;

    size_t *w_size;

    int lines;
    int w_res;

    TRACE();

    if (x_values == NULL)
        ERROR("x_values == NULL\n", 1);

    if (y_values == NULL)
        ERROR("y_values == NULL\n", 1);

    if (x_len != y_len)
        ERROR("x_len != y_len\n", 1);

    if (title == NULL)
        ERROR("Title == NULL\n", 1);

    if (x_legend == NULL)
        ERROR("x_legend == NULL\n", 1);

    if (y_legend == NULL)
        ERROR("y_legend == NULL\n", 1);

    if (reset)
        reset_terminal();

    if (get_terminal_size(&width, &height))
        ERROR("Get terminal size error\n", 1);

    LOG("Terminal Size: W = %zu, H = %zu\n", width, height);

    lines = draw_legend(title, x_legend, y_legend, width, height);
    if (lines < 0)
        ERROR("Draw legend error\n", 1);

    LOG("Legend consume %d lines\n", lines);
    height -= (size_t)lines;

    if (reserve_w_h_for_const_str(&width, &height))
        ERROR("Cannot reserve lines and columns\n", 1);

    w_res = reserve_width_for_y_numbers(y_values, y_len, width);
    if (w_res < 0)
        ERROR("Cant reserve space for Y-Axis values\n", 1);

    LOG("Resevered %d columns for y numbers\n", w_res);
    width -= (size_t)w_res;

    LOG("Write x to buffer\n");
    buffer_x = write_x_to_buffer(x_values, x_len, width, &buffer_x_size);
    if (buffer_x == NULL)
        ERROR("To many x values for this width %zu\n", 1, width);

    LOG("Resize x axis\n");
    w_size = resize_x(buffer_x, x_values, buffer_x_size, x_len);
    if (w_size == NULL)
    {
        FREE(buffer_x);
        ERROR("Cannot resize X Axis\n", 1);
    }

    if (draw_y_axis(y_values, y_len, height, (size_t)w_res, w_size))
    {
        FREE(buffer_x);
        FREE(w_size);
        ERROR("Cannot draw Histogram\n", 1);
    }

    draw_x_axis(buffer_x, (size_t)w_res);

    FREE(buffer_x);
    FREE(w_size);

    return 0;
}
