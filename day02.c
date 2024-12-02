#include "basic.c"
#include "string.h"

i32 part_1(u8* content, u32 file_size) {
    i32 levels[20] = {0};
    i32 level_count = 0;
    i8 direction = 0;
    b8 safe = true;
    i32 safe_count = 0;

    for (i32 i = 0; i < file_size; i++) {
        if (content[i] >= '0' && content[i] <= '9') {
            levels[level_count] *= 10;
            levels[level_count] += content[i] - '0';
        } else if (content[i] == ' ' || content[i] == '\n') {
            if (level_count == 1) {
                direction = levels[1] - levels[0];
            }

            if (level_count > 0) {
                i32 distance = levels[level_count] - levels[level_count-1];
                i32 abs_distance = distance;
                if (abs_distance < 0) abs_distance *= -1;

                if (distance == 0 ||
                        (abs_distance > 3) ||
                        (direction < 0 && levels[level_count] > levels[level_count-1]) ||
                        (direction > 0 && levels[level_count] < levels[level_count-1])) {
                    safe = false;
                }
            }

            level_count++;

            if (content[i] == '\n') {
                if (safe) safe_count++;
                level_count = 0;
                for (i32 m = 0; m < 20; m++) { levels[m] = 0; }
                safe = true;
            }
        }
    }

    return safe_count;
}

i32 part_2(u8* content, u32 file_size) {
    i32 levels[20] = {0};
    i32 level_count = 0;
    i8 direction = 0;
    b8 safe = true;
    i32 safe_count = 0;

    for (i32 i = 0; i < file_size; i++) {
        if (content[i] >= '0' && content[i] <= '9') {
            levels[level_count] *= 10;
            levels[level_count] += content[i] - '0';
        } else if (content[i] == ' ') {
            level_count++;
        } else if (content[i] == '\n') {
            level_count++;

            for (i32 skip_index = -1; skip_index < level_count; skip_index++) {
                safe = true;

                direction = 0;
                for (i32 l = 0; l < level_count; l++) {
                    if (l == skip_index) continue;
                    i32 pl = l-1;
                    if (pl == skip_index) pl--;
                    if (pl < 0) continue;

                    if (direction == 0) {
                        direction = levels[l] - levels[pl];
                    }
                    i32 distance = levels[l] - levels[pl];
                    i32 abs_distance = distance;
                    if (abs_distance < 0) abs_distance *= -1;

                    if (distance == 0 ||
                            (abs_distance > 3) ||
                            (direction < 0 && levels[l] > levels[pl]) ||
                            (direction > 0 && levels[l] < levels[pl])) {
                        safe = false;
                    }
                }

                if (safe) {
                    safe_count++;
                    break;
                }
            }

            level_count = 0;
            for (i32 m = 0; m < 20; m++) levels[m] = 0;
        }
    }

    return safe_count;
}

i32 main() {
    u32 file_size;
    u8* content = load_file("input02.txt", &file_size);

    printf("part 1: %i\n", part_1(content, file_size));
    printf("part 2: %i\n", part_2(content, file_size));

    return 0;
}
