#include "basic.c"
u32 width, height;

typedef struct { i32 x, y; } v2;
v2 add(v2 a, v2 b) { return (v2){ a.x+b.x, a.y+b.y }; }
u8 tile(u8* content, v2 pos) {
    if (pos.x < 0 || pos.x >= width-1) return '.';
    if (pos.y < 0 || pos.y >= height) return '.';
    return content[pos.y*width + pos.x];
}

u64 part_1(u8* content, u32 file_size) {
    v2 stack[1000];
    u32 stack_count = 0;

    i32 sum = 0;
    for (i32 i = 0; i < file_size; i++) {
        if (content[i] >= 'A' && content[i] <= 'Z') {
            stack[stack_count++] = (v2){ i % width, i / width };
        }

        i32 squares = 0;
        i32 fences = 0;
        while (stack_count > 0) {
            v2 pos = stack[--stack_count];

            u8 c = tile(content, pos);
            if (c >= 'a' && c <= 'z') continue;
            squares++;

            u8 lower_c = c + ('a'-'A');
            v2 directions[4] = {
                (v2){  0, -1 },
                (v2){ +1,  0 },
                (v2){  0, +1 },
                (v2){ -1,  0 }
            };
            for (i32 p = 0; p < 4; p++) {
                v2 next_pos = add(pos, directions[p]);
                u8 next_c = tile(content, next_pos);

                if (next_c == c) {
                    stack[stack_count++] = next_pos;
                } else if (next_c != lower_c) {
                    fences++;
                }
            }

            content[pos.y*width+pos.x] = lower_c;
        }

        sum += squares * fences;
    }

    return sum;
}

u64 part_2(u8* content, u32 file_size) {
    v2 stack[1000];
    u32 stack_count = 0;

    i32 sum = 0;
    for (i32 i = 0; i < file_size; i++) {
        if (content[i] >= 'A' && content[i] <= 'Z') {
            stack[stack_count++] = (v2){ i % width, i / width };
        }

        i32 squares = 0;
        i32 fences = 0;
        while (stack_count > 0) {
            v2 pos = stack[--stack_count];

            u8 c = tile(content, pos);
            if (c >= 'a' && c <= 'z') continue;
            squares++;

            u8 lower_c = c + ('a'-'A');
            v2 directions[8] = {
                (v2){  0, -1 },
                (v2){ +1, -1 },
                (v2){ +1,  0 },
                (v2){ +1, +1 },
                (v2){  0, +1 },
                (v2){ -1, +1 },
                (v2){ -1,  0 },
                (v2){ -1, -1 }
            };
            for (i32 p = 0; p < 8; p++) {
                if (p%2 != 0) continue;

                v2 next_pos = add(pos, directions[p]);
                u8 next_c = tile(content, next_pos);

                if (next_c == c) {
                    stack[stack_count++] = next_pos;
                }
            }

            for (i32 r = 0; r < 4; r++) {
                b8 b[3];
                for (i32 p = 0; p < 3; p++) {
                    v2 next_pos = add(pos, directions[(p+(r*2)) % 8]);
                    u8 next_c = tile(content, next_pos);
                    b[p] = (next_c == c || next_c == lower_c);
                }
                if (!b[0] && !b[1] && !b[2]) { fences++; }
                if ( b[0] && !b[1] &&  b[2]) { fences++; }
                if (!b[0] &&  b[1] && !b[2]) { fences++; }
            }

            content[pos.y*width+pos.x] = lower_c;
        }

        sum += squares * fences;
    }

    return sum;
}

i32 main() {
    u32 file_size;
    u8* content = load_file_wh("input12.txt", &file_size, &width, &height);
    printf("part 1: %llu\n", part_1(content, file_size));
    content = load_file_wh("input12.txt", &file_size, &width, &height);
    printf("part 2: %llu\n", part_2(content, file_size));

    return 0;
}
