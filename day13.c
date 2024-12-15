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
            printf("\n");

            for (i32 j = 0; j < 6; j++) numbers[j] = 0;
            number_count = 0;
        }
    }

    return sum;
}

u64 part_2(u8* content, u32 file_size) {
    return 0;
}

i32 main() {
    u32 file_size;
    u8* content = load_file("input13.txt", &file_size);
    printf("part 1: %llu\n", part_1(content, file_size));
    printf("part 2: %llu\n", part_2(content, file_size));

    return 0;
}
