#include "basic.c"
#include <string.h>

typedef struct { i32 x, y; } v2;
b8 v2_eq(v2 a, v2 b) { return (a.x == b.x && a.y == b.y); }
v2 v2_add(v2 a, v2 b) { return (v2){ a.x+b.x, a.y+b.y }; }
v2 v2_sub(v2 a, v2 b) { return (v2){ a.x-b.x, a.y-b.y }; }
v2 keypad_coords[11] = {{1,3},{0,2},{1,2},{2,2},{0,1},{1,1},{2,1},{0,0},{1,0},{2,0},{2,3}};
v2 joypad_coords[5] = {{1,0},{2,1},{1,1},{0,1},{2,0}};
v2 keypad(u8 c) {
    if (c == 'A') return keypad_coords[10];
    if (c == ' ') return (v2){ 0, 3 };
    return keypad_coords[c-'0'];
}
v2 joypad(u8 c) {
    if (c == ' ') return (v2){ 0, 0 };
    if (c == '^') return joypad_coords[0];
    if (c == '>') return joypad_coords[1];
    if (c == 'v') return joypad_coords[2];
    if (c == '<') return joypad_coords[3];
    return joypad_coords[4];
}

u64 part_1(u8* content, u32 file_size) {
    v2 robot[3] = { keypad('A'), joypad('A'), joypad('A') };

    u8* code = (u8*)malloc(1000);
    u8* code_new = (u8*)malloc(1000);
    i32 code_count = 0;
    i32 code_new_count = 0;

    i32 n = 0;
    i32 sum = 0;
    for (i32 i = 0; i < file_size; i++) {
        if (content[i] != '\n') {
            code[code_count++] = content[i];
            if (content[i] != 'A') {
                n *= 10;
                n += content[i]-'0';
            }
        } else {
            for (i32 r = 0; r < 3; r++) {
                for (i32 c = 0; c < code_count; c++) {
                    v2 coord = (r == 0) ? keypad(code[c]) : joypad(code[c]);
                    v2 gap = (r == 0) ? (v2){0,3} : (v2){0,0};
                    v2 dt = v2_sub(coord, robot[r]);
                    b8 x_issue = robot[r].y == gap.y && robot[r].x+dt.x == gap.x;
                    b8 y_issue = robot[r].x == gap.x && robot[r].y+dt.y == gap.y;

                    i32 x = dt.x;
                    i32 y = dt.y;
                    if (!x_issue) for (; x < 0; x++) code_new[code_new_count++] = '<';

                    if (!y_issue) for (; y < 0; y++) code_new[code_new_count++] = '^';
                    if (!y_issue) for (; y > 0; y--) code_new[code_new_count++] = 'v';

                    for (; x < 0; x++) code_new[code_new_count++] = '<';
                    for (; x > 0; x--) code_new[code_new_count++] = '>';

                    for (; y < 0; y++) code_new[code_new_count++] = '^';
                    for (; y > 0; y--) code_new[code_new_count++] = 'v';

                    code_new[code_new_count++] = 'A';
                    robot[r] = coord;
                }

                u8* tmp = code;
                code = code_new;
                code_new = tmp;
                code_count = code_new_count;
                code_new_count = 0;

                code[code_count] = 0;
            }

            sum += n * code_count;
            n = 0;
            code_count = 0;
            code_new_count = 0;
            robot[0] = keypad('A');
            robot[1] = joypad('A');
            robot[2] = joypad('A');
        }
    }

    return sum;
}

u64 part_2(u8* content, u32 file_size) {
    return 0;
}

i32 main() {
    u32 file_size;
    u8* content = load_file("input21.txt", &file_size);
    printf("part 1: %llu\n", part_1(content, file_size));
    printf("part 2: %llu\n", part_2(content, file_size));

    return 0;
}
