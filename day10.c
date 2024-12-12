#include "basic.c"
#define WIDTH 59
#define HEIGHT 58

typedef struct { i32 x, y; } v2;
typedef struct { v2 start; v2 end; } StackNode;

b8 v2_eq(v2 a, v2 b) {
    return a.x == b.x && a.y == b.y;
}
b8 node_eq(StackNode a, StackNode b) {
    return v2_eq(a.start, b.start) && v2_eq(a.end, b.end);
}
i8 tile(u8* content, v2 p) {
    if (p.x < 0 || p.x >= WIDTH-1) return -1;
    if (p.y < 0 || p.y >= HEIGHT) return -1;
    u8 c = content[p.y*WIDTH+p.x];
    return c-'0';
}

i32 unique_count = 0;
i32 total_count = 0;
u64 part_1(u8* content, u32 file_size) {
    StackNode stack[5000];
    i32 stack_index = 0;

    for (i32 i = 0; i < file_size; i++) {
        if (content[i] == '0') {
            v2 pos = (v2){ i % WIDTH, i / WIDTH };
            stack[stack_index++] = (StackNode){ pos, pos };
        }
    }

    StackNode paths[2000];
    i32 trailhead_count = 0;
    while (stack_index > 0) {
        StackNode node = stack[--stack_index];

        v2 position = node.end;
        i8 current_number = tile(content, position);
        if (current_number == 9) {
            b8 unique = true;
            for (i32 i = 0; i < unique_count; i++) {
                if (node_eq(paths[i], node)) {
                    unique = false;
                    break;
                }
            }

            total_count++;
            if (unique) {
                paths[unique_count++] = node;
            }

            continue;
        }

        v2 deltas[4] = {
            (v2){  0, -1 },
            (v2){ +1,  0 },
            (v2){  0, +1 },
            (v2){ -1,  0 }
        };
        for (i32 i = 0; i < 4; i++) {
            v2 next_position = (v2){ position.x+deltas[i].x, position.y+deltas[i].y };
            i8 next_number = tile(content, next_position);
            if (next_number == current_number+1) {
                stack[stack_index++] = (StackNode){ node.start, next_position };
            }
        }
    }

    return unique_count;
}

u64 part_2(u8* content, u32 file_size) {
    return total_count;
}

i32 main() {
    u32 file_size;
    u8* content = load_file("input10.txt", &file_size);
    printf("part 1: %llu\n", part_1(content, file_size));
    printf("part 2: %llu\n", part_2(content, file_size));

    return 0;
}
