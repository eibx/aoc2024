#include "basic.c"
#include <math.h>

typedef struct { u64 x, y; } v2;
u64 part_1(u8* content, u32 file_size) {
    i32 numbers[6] = {0};
    i32 number_count = 0;
    i32 games = 1;
    u64 sum = 0;
    for (i32 i = 0; i < file_size; i++) {
        if (content[i] >= '0' && content[i] <= '9') {
            numbers[number_count] *= 10;
            numbers[number_count] += content[i] - '0';
        } else if (numbers[number_count] > 0) {
            number_count++;
        }

        if (number_count == 6) {
            i32 cheapest = 999999;
            v2 a = (v2){ numbers[0], numbers[1] };
            v2 b = (v2){ numbers[2], numbers[3] };
            v2 goal = (v2){ numbers[4], numbers[5] };

            for (i32 k = 0; k < 100; k++) {
                for (i32 j = 0; j < 100; j++) {
                    v2 distance = (v2){ a.x*k + b.x*j, a.y*k + b.y*j };

                    if (distance.x == goal.x && distance.y == goal.y) {
                        i32 tokens = k*3 + j;
                        if (tokens < cheapest) cheapest = tokens;
                    }
                }
            }

            if (cheapest < 999999) {
                sum += cheapest;
            }

            for (i32 j = 0; j < 6; j++) numbers[j] = 0;
            number_count = 0;
        }
    }

    return sum;
}

u64 part_2(u8* content, u32 file_size) {
    i32 numbers[6] = {0};
    i32 number_count = 0;
    i32 games = 1;
    u64 sum = 0;
    for (i32 i = 0; i < file_size; i++) {
        if (content[i] >= '0' && content[i] <= '9') {
            numbers[number_count] *= 10;
            numbers[number_count] += content[i] - '0';
        } else if (numbers[number_count] > 0) {
            number_count++;
        }

        if (number_count == 6) {
            f64 ax = (f64)numbers[0];
            f64 ay = (f64)numbers[1];
            f64 bx = (f64)numbers[2];
            f64 by = (f64)numbers[3];
            f64 goalx = (f64)(numbers[4] + 10000000000000);
            f64 goaly = (f64)(numbers[5] + 10000000000000);

            f64 determinant = (ax * by) - (ay * bx);
            f64 t = (goalx*by - goaly*bx) / determinant;

            f64 intersection_x = ax * t;
            f64 intersection_y = ay * t;

            f64 ax_times = intersection_x/ax;
            f64 bx_times = (goalx-intersection_x)/bx;

            f64 episilon = 0.001;
            b8 ax_divides = (ax_times - floor(ax_times)) < episilon;
            b8 bx_divides = (bx_times - floor(bx_times)) < episilon;

            if (ax_divides && bx_divides) {
                sum += ax_times*3 + bx_times;
            }

            for (i32 j = 0; j < 6; j++) numbers[j] = 0;
            number_count = 0;
        }
    }

    return sum;
}

i32 main() {
    u32 file_size;
    u8* content = load_file("input13.txt", &file_size);
    printf("part 1: %llu\n", part_1(content, file_size));
    printf("part 2: %llu\n", part_2(content, file_size));

    return 0;
}
