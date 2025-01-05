#include "basic.c"
#include <string.h>

u64 part_1(u8* content, u32 file_size) {
    u8 state = 0;

    u8* options[500] = {};
    i32 options_count = 0;
    i32 option_length = 0;

    i32 pattern_stack[1000];
    i32 pattern_stack_count = 0;

    i32 sum = 0;
    for (i32 i = 0; i < file_size; i++) {
        if (state == 0) {
            if (content[i] >= 'a' && content[i] <= 'z') {
                option_length++;
            } else if (content[i] == ',' || content[i] == '\n') {
                if (option_length == 0) { state++; continue; }
                content[i] = 0;
                options[options_count++] = content + i - option_length;
                option_length = 0;
            }
        } else if (state == 1) {
            pattern_stack[pattern_stack_count++] = i;

            b8 match = false;
            while (pattern_stack_count > 0) {
                i32 current_index = pattern_stack[--pattern_stack_count];
                for (i32 o = 0; o < options_count; o++) {
                    option_length = strlen(options[o]);
                    if (strncmp(options[o], content+current_index, option_length) == 0) {
                        i32 new_index = current_index + option_length;
                        if (content[new_index] != '\n') {
                            pattern_stack[pattern_stack_count++] = current_index + option_length;
                        } else {
                            match = true;
                            sum++;
                            break;
                        }
                    }
                }
                if (match) break;
            }

            pattern_stack_count = 0;
            state++;
        } else if (state == 2) {
            if (content[i] == '\n') { state--; }
        }
    }

    return sum;
}

u8* options[500] = {};
i32 options_count = 0;

i64 solve(u8* pattern, i32 pattern_left, i64* cache) {
    if (pattern_left == 0) return 1;

    if (cache[pattern_left] == -1) return 0;
    if (cache[pattern_left] > 0) return cache[pattern_left];

    i64 matches = 0;
    for (int i = 0; i < options_count; i++) {
        i32 option_length = strlen(options[i]);
        if (pattern_left < option_length) continue;

        if (strncmp(pattern, options[i], option_length) == 0) {
            matches += solve(pattern + option_length, pattern_left - option_length, cache);
        }
    }
    cache[pattern_left] = (matches > 0) ? matches : -1;
    return matches;
}

u64 part_2(u8* content, u32 file_size) {
    u8 state = 0;

    i32 option_length = 0;
    u64 sum = 0;
    for (i32 i = 0; i < file_size; i++) {
        if (state == 0) {
            if (content[i] >= 'a' && content[i] <= 'z') {
                option_length++;
            } else if (content[i] == ',' || content[i] == '\n') {
                if (option_length == 0) { state++; continue; }
                content[i] = 0;
                options[options_count++] = content + i - option_length;
                option_length = 0;
            }
        } else if (state == 1) {
            i32 length = 0;
            for (i32 j = 0; j < file_size; j++) {
                length = j;
                if (content[i+j] == '\n') break;
            }
            u64 cache[1024] = {0};
            sum += solve(&content[i], length, cache);
            state++;
        } else if (state == 2) {
            if (content[i] == '\n') { state--; }
        }
    }

    return sum;
}

i32 main() {
    u32 file_size;
    u8* content = load_file("input19.txt", &file_size);
    printf("part 1: %llu\n", part_1(content, file_size));
    content = load_file("input19.txt", &file_size);
    printf("part 2: %llu\n", part_2(content, file_size));

    return 0;
}
