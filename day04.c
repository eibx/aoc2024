#include "basic.c"
#define WIDTH 141

u8* c;
i32 fs;
u8 pos(i32 x, i32 y) {
    if (x > WIDTH) return 0;
    if (y > fs / WIDTH) return 0;
    return c[y*WIDTH + x];
}

i32 part_1(u8* content, u32 file_size) {
    c = content;
    fs = file_size;
    i32 count = 0;
    for (i32 i = 0; i < file_size; i++) {
        i32 x = i % WIDTH;
        i32 y = i / WIDTH;
        if (content[i] == 'X') {
            if (pos(x+1, y) == 'M' && pos(x+2, y) == 'A' && pos(x+3, y) == 'S') count++;
            if (pos(x-1, y) == 'M' && pos(x-2, y) == 'A' && pos(x-3, y) == 'S') count++;
            if (pos(x, y+1) == 'M' && pos(x, y+2) == 'A' && pos(x, y+3) == 'S') count++;
            if (pos(x, y-1) == 'M' && pos(x, y-2) == 'A' && pos(x, y-3) == 'S') count++;

            if (pos(x-1, y-1) == 'M' && pos(x-2, y-2) == 'A' && pos(x-3, y-3) == 'S') count++;
            if (pos(x+1, y-1) == 'M' && pos(x+2, y-2) == 'A' && pos(x+3, y-3) == 'S') count++;
            if (pos(x+1, y+1) == 'M' && pos(x+2, y+2) == 'A' && pos(x+3, y+3) == 'S') count++;
            if (pos(x-1, y+1) == 'M' && pos(x-2, y+2) == 'A' && pos(x-3, y+3) == 'S') count++;
        }
    }
    return count;
}

i32 part_2(u8* content, u32 file_size) {
    c = content;
    fs = file_size;
    i32 count = 0;
    for (i32 i = 0; i < file_size; i++) {
        i32 x = i % WIDTH;
        i32 y = i / WIDTH;
        if (content[i] == 'A') {
            i32 lines = 0;
            for (i32 dx = -1; dx <= 1; dx += 2) {
                for (i32 dy = -1; dy <= 1; dy += 2) {
                    if (pos(x+dx, y+dy) == 'M' && pos(x+(dx*-1), y+(dy*-1)) == 'S')
                        lines++;
                }
            }
            if (lines == 2) count++;
        }
    }
    return count;
}

i32 main() {
    u32 file_size;
    u8* content = load_file("input04.txt", &file_size);

    printf("part 1: %i\n", part_1(content, file_size));
    printf("part 2: %i\n", part_2(content, file_size));

    return 0;
}
