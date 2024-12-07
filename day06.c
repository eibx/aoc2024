#include "basic.c"
#define WIDTH 131
#define HEIGHT 130

typedef struct { i32 x, y; } v2;
v2 add(v2 a, v2 b) { return (v2){ a.x+b.x, a.y+b.y }; }
v2 rotate(v2 a) {
    return (a.y == 0) ? (v2){ 0, a.x } : (v2){ a.y*-1, 0 };
}

v2 start_pos;
v2 start_dir;

i32 part_1(u8* content, u32 file_size) {
    for (i32 i = 0; i < file_size; i++) {
        if (content[i] == '^' || content[i] == '>' || content[i] == 'V' || content[i] == '<') {
            start_pos = (v2){ i % WIDTH, i / WIDTH };
            if (content[i] == '^') start_dir = (v2){  0,-1 };
            if (content[i] == '>') start_dir = (v2){ +1, 0 };
            if (content[i] == 'V') start_dir = (v2){  0,+1 };
            if (content[i] == '<') start_dir = (v2){ -1, 0 };
            break;
        }
    }

    v2 guard_pos = start_pos;
    v2 guard_dir = start_dir;
    while (true) {
        content[guard_pos.y * WIDTH + guard_pos.x] = 'X';

        v2 new_pos = add(guard_pos, guard_dir);

        if (new_pos.x < 0 || new_pos.x > WIDTH)  break;
        if (new_pos.y < 0 || new_pos.y > HEIGHT) break;

        i32 new_i = (new_pos.y * WIDTH) + new_pos.x;
        if (content[new_i] == '#') {
            guard_dir = rotate(guard_dir);
        } else {
            guard_pos = new_pos;
        }
    }

    i32 xes = 0;
    for (i32 i = 0; i < file_size; i++) {
        if (content[i] == 'X') xes++;
    }

    return xes;
}

u8* c;
u32 fs;
b8 is_looping(v2 pos, v2 dir) {
    i32 moves = 0;
    while (true) {
        v2 new_pos = add(pos, dir);

        if (new_pos.x < 0 || new_pos.x >= WIDTH)  break;
        if (new_pos.y < 0 || new_pos.y >= HEIGHT) break;

        i32 new_i = (new_pos.y * WIDTH) + new_pos.x;
        if (c[new_i] == '#' || c[new_i] == 'O') {
            dir = rotate(dir);
        } else {
            pos = new_pos;
            if (moves++ > WIDTH*HEIGHT*2) return true;
        }
    }

    return false;
}

i32 part_2(u8* content, u32 file_size) {
    c = content;
    fs = file_size;

    v2 guard_pos = start_pos;
    v2 guard_dir = start_dir;

    i32 loops = 0;
    while (true) {
        v2 new_pos = add(guard_pos, guard_dir);

        if (new_pos.x < 0 || new_pos.x > WIDTH)  break;
        if (new_pos.y < 0 || new_pos.y > HEIGHT) break;

        i32 new_i = (new_pos.y * WIDTH) + new_pos.x;
        if (content[new_i] == '#') {
            guard_dir = rotate(guard_dir);
        } else {
            if (content[new_i] != ',') {
                content[new_i] = 'O';
                if (is_looping(guard_pos, guard_dir)) loops++;

                for (i32 i = 0; i < file_size; i++) {
                    if (content[i] != '#' && content[i] != '\n' && content[i] != ',')
                        content[i] = '.';
                }
                content[new_i] = ',';
            }

            guard_pos = new_pos;
        }
    }


    return loops;
}

i32 main() {
    u32 file_size;
    u8* content = load_file("input06.txt", &file_size);
    printf("part 1: %i\n", part_1(content, file_size));
    printf("part 2: %i\n", part_2(content, file_size));

    return 0;
}
