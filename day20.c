#include "basic.c"
#define WIDTH 142+2
#define HEIGHT 142+2

typedef struct { i32 x, y; } v2;
u64 part_1(u8* content, u32 file_size) {
    i32 (*map)[HEIGHT] = malloc(sizeof(i32[WIDTH][HEIGHT]));
    for (i32 mx = 0; mx < WIDTH; mx++) {
        for (i32 my = 0; my < HEIGHT; my++) {
            map[mx][my] = -2;
        }
    }

    i32 x, y;
    for (i32 i = 0; i < file_size; i++) {
        i32 mx = (i%WIDTH);
        i32 my = (i/HEIGHT);
        if (content[i] == '#') map[mx][my] = -2;
        if (content[i] == '.') map[mx][my] = -1;
        if (content[i] == 'E') map[mx][my] = -1;
        if (content[i] == 'S') {
            map[mx][my] = 0;
            x = mx;
            y = my;
        }
    }

    i32 steps = 1;
    i32 cheats = 0;
    while (true) {
        i32 previous_steps = steps;
        i32 c;

        if (map[x+0][y-2] >= 0) { c = map[x][y]-map[x+0][y-2]-2; if (c >= 100) cheats++; }
        if (map[x+2][y+0] >= 0) { c = map[x][y]-map[x+2][y+0]-2; if (c >= 100) cheats++; }
        if (map[x+0][y+2] >= 0) { c = map[x][y]-map[x+0][y+2]-2; if (c >= 100) cheats++; }
        if (map[x-2][y+0] >= 0) { c = map[x][y]-map[x-2][y+0]-2; if (c >= 100) cheats++; }

        if      (map[x+0][y-1] == -1) { map[x+0][y-1] = steps++; y--; }
        else if (map[x+1][y+0] == -1) { map[x+1][y+0] = steps++; x++; }
        else if (map[x+0][y+1] == -1) { map[x+0][y+1] = steps++; y++; }
        else if (map[x-1][y+0] == -1) { map[x-1][y+0] = steps++; x--; }

        if (steps == previous_steps) break;
    }

    return cheats;
}

u64 part_2(u8* content, u32 file_size) {
    i32 (*map)[HEIGHT] = malloc(sizeof(i32[WIDTH][HEIGHT]));
    for (i32 mx = 0; mx < WIDTH; mx++) {
        for (i32 my = 0; my < HEIGHT; my++) {
            map[mx][my] = -2;
        }
    }

    i32 x, y;
    for (i32 i = 0; i < file_size; i++) {
        i32 mx = (i%WIDTH);
        i32 my = (i/HEIGHT);
        if (content[i] == '#') map[mx][my] = -2;
        if (content[i] == '.') map[mx][my] = -1;
        if (content[i] == 'E') map[mx][my] = -1;
        if (content[i] == 'S') {
            map[mx][my] = 0;
            x = mx;
            y = my;
        }
    }

    i32 steps = 1;
    i32 cheats = 0;
    i32 distance = 20;
    while (true) {
        i32 previous_steps = steps;

        for (i32 dx = -distance; dx <= distance; ++dx) {
            i32 y_distance = distance - abs(dx);
            for (i32 dy = -y_distance; dy <= y_distance; ++dy) {
                if (x+dx < 0 || x+dx >= WIDTH || y+dy < 0 || y+dy >= HEIGHT) continue;
                if (map[x+dx][y+dy] >= 0) {
                    i32 cheat = map[x][y] - map[x+dx][y+dy] - (abs(dx)+abs(dy));
                    if (cheat >= 100) {
                        cheats++;
                    }
                }
            }
        }

        if      (map[x+0][y-1] == -1) { map[x+0][y-1] = steps++; y--; }
        else if (map[x+1][y+0] == -1) { map[x+1][y+0] = steps++; x++; }
        else if (map[x+0][y+1] == -1) { map[x+0][y+1] = steps++; y++; }
        else if (map[x-1][y+0] == -1) { map[x-1][y+0] = steps++; x--; }

        if (steps == previous_steps) break;
    }

    return cheats;
}

i32 main() {
    u32 file_size;
    u8* content = load_file("input20.txt", &file_size);
    printf("part 1: %llu\n", part_1(content, file_size));
    content = load_file("input20.txt", &file_size);
    printf("part 2: %llu\n", part_2(content, file_size));

    return 0;
}
