#include "basic.c"

u64 part_1(u8* content, u32 file_size) {
    u8 keys[1000][5] = {};
    u8 locks[1000][5] = {};
    i32 keys_count = 0;
    i32 locks_count = 0;

    i32 row = 0;
    i32 col = 0;
    b8 is_key = false;
    u8 pattern[5] = {};
    for (i32 i = 0; i < file_size; i++) {
        if (row == 0 && col == 0) {
            is_key = (content[i] == '.');
        }
        if (row > 0 && row < 6) {
            if (content[i] == '#')
                pattern[col]++;
        }

        col++;
        if (content[i] == '\n') {
            if (content[i+1] == '\n') {
                if (row > 0) {
                    if (is_key) {
                        for (i32 n = 0; n < 5; n++) keys[keys_count][n] = pattern[n];
                        keys_count++;
                    } else {
                        for (i32 n = 0; n < 5; n++) locks[locks_count][n] = pattern[n];
                        locks_count++;
                    }
                }

                col = 0;
                row = 0;
                i++;
                for (i32 n = 0; n < 5; n++) pattern[n] = 0;
            } else {
                col = 0;
                row++;
            }
        }
    }
    if (row > 0) {
        if (is_key) {
            for (i32 n = 0; n < 5; n++) keys[keys_count][n] = pattern[n];
            keys_count++;
        } else {
            for (i32 n = 0; n < 5; n++) locks[locks_count][n] = pattern[n];
            locks_count++;
        }
    }


    i32 sum = 0;
    for (i32 l = 0; l < locks_count; l++) {
        for (i32 k = 0; k < keys_count; k++) {
            b8 valid = true;
            for (i32 n = 0; n < 5; n++) {
                if (locks[l][n]+keys[k][n] > 5) {
                    valid = false;
                    break;
                }
            }
            if (valid) sum++;
        }
    }


    return sum;
}

u64 part_2(u8* content, u32 file_size) {
    return 0;
}

i32 main() {
    u32 file_size;
    u8* content = load_file("input25.txt", &file_size);
    printf("part 1: %llu\n", part_1(content, file_size));
    printf("part 2: %llu\n", part_2(content, file_size));

    return 0;
}
