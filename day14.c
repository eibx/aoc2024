#include "basic.c"
i32 width = 101;
i32 height = 103;

typedef struct { i32 x, y; } v2;
typedef struct { v2 position; v2 velocity; } Robot;
u64 part_1(u8* content, u32 file_size) {
    Robot robots[1000];
    i32 robot_count = 0;

    i32 numbers[4];
    i32 number_count = 0;
    b8 negative = false;
    for (i32 j = 0; j < 4; j++) numbers[j] = -1337;

    for (i32 i = 0; i < file_size; i++) {
        if (content[i] == '-') {
            negative = true;
        } else if (content[i] >= '0' && content[i] <= '9') {
            if (numbers[number_count] == -1337) numbers[number_count] = 0;
            numbers[number_count] *= 10;
            numbers[number_count] += content[i] - '0';
        } else if (numbers[number_count] != -1337) {
            if (negative) numbers[number_count] *= -1;
            number_count++;
            negative = false;
        }

        if (number_count == 4) {
            robots[robot_count++] = (Robot){
                (v2){ numbers[0], numbers[1] },
                (v2){ numbers[2], numbers[3] }
            };
            number_count = 0;
            for (i32 j = 0; j < 4; j++) numbers[j] = -1337;
        }
    }

    for (i32 i = 0; i < 100; i++) {
        for (i32 r = 0; r < robot_count; r++) {
            v2* p = &robots[r].position;
            v2* v = &robots[r].velocity;
            p->x += v->x;
            p->y += v->y;
            p->x = p->x % (width);
            p->y = p->y % (height);
            p->x = p->x < 0 ? p->x + (width) : p->x;
            p->y = p->y < 0 ? p->y + (height) : p->y;
        }
    }

    u64 quads[4] = {0};
    for (i32 r = 0; r < robot_count; r++) {
        v2 p = robots[r].position;
        if (p.x < width/2 && p.y < height/2) quads[0]++;
        if (p.x > width/2 && p.y < height/2) quads[1]++;
        if (p.x < width/2 && p.y > height/2) quads[2]++;
        if (p.x > width/2 && p.y > height/2) quads[3]++;
    }

    return quads[0]*quads[1]*quads[2]*quads[3];
}

u64 part_2(u8* content, u32 file_size) {
    Robot robots[1000];
    i32 robot_count = 0;

    i32 numbers[4];
    i32 number_count = 0;
    b8 negative = false;
    for (i32 j = 0; j < 4; j++) numbers[j] = -1337;

    for (i32 i = 0; i < file_size; i++) {
        if (content[i] == '-') {
            negative = true;
        } else if (content[i] >= '0' && content[i] <= '9') {
            if (numbers[number_count] == -1337) numbers[number_count] = 0;
            numbers[number_count] *= 10;
            numbers[number_count] += content[i] - '0';
        } else if (numbers[number_count] != -1337) {
            if (negative) numbers[number_count] *= -1;
            number_count++;
            negative = false;
        }

        if (number_count == 4) {
            robots[robot_count++] = (Robot){
                (v2){ numbers[0], numbers[1] },
                (v2){ numbers[2], numbers[3] }
            };
            number_count = 0;
            for (i32 j = 0; j < 4; j++) numbers[j] = -1337;
        }
    }

    for (i32 i = 0; i < 10000; i++) {
        u8 m[width][height];
        for (i32 my = 0; my < width; my++) {
            for (i32 mx = 0; mx < width; mx++) {
                m[mx][my] = '.';
            }
        }

        for (i32 r = 0; r < robot_count; r++) {
            v2* p = &robots[r].position;
            v2* v = &robots[r].velocity;
            p->x += v->x;
            p->y += v->y;
            p->x = p->x % (width);
            p->y = p->y % (height);
            p->x = p->x < 0 ? p->x + (width) : p->x;
            p->y = p->y < 0 ? p->y + (height) : p->y;
            m[p->x][p->y] = 'X';
        }

        for (i32 my = 0; my < width; my++) {
            i32 consecutive_x_robots = 0;
            for (i32 mx = 0; mx < width; mx++) {
                if (mx > 0 && m[mx][my] == 'X' && m[mx-1][my] == 'X') {
                    consecutive_x_robots++;
                }
            }

            if (consecutive_x_robots > 20) return i+1;
            consecutive_x_robots = 0;
        }
    }

    return 0;
}

i32 main() {
    u32 file_size;
    u8* content = load_file("input14.txt", &file_size);
    printf("part 1: %llu\n", part_1(content, file_size));
    printf("part 2: %llu\n", part_2(content, file_size));

    return 0;
}
