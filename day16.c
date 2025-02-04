#include "basic.c"
#define I32_MAX 2147483647
i32 width, height;

typedef struct { i32 x, y; } v2;
b8 v2_eq(v2 a, v2 b) { return (a.x == b.x && a.y == b.y); }
v2 v2_flip(v2 a) { return (v2){ a.x*-1, a.y*-1 }; }
v2 v2_add(v2 a, v2 b) { return (v2){ a.x+b.x, a.y+b.y }; }

typedef struct { v2 position; v2 direction; i32 value; } StackNode;

v2 directions[4] = {
    (v2){ 0, -1 },
    (v2){ 1, 0 },
    (v2){ 0, 1 },
    (v2){ -1, 0 }
};
v2 start, end;
i32* map;

u64 part_1(u8* content, u32 file_size) {
    map = (i32*)malloc(file_size*sizeof(i32));
    for (i32 i = 0; i < file_size; i++) {
        if (content[i] == 'S') {
            start.x = i % width;
            start.y = i / width;
        } else if (content[i] == 'E') {
            end.x = i % width;
            end.y = i / width;
        }

        if (content[i] == '#') {
            map[i] = -1;
        } else {
            map[i] = I32_MAX;
        }
    }
    map[start.y*width+start.x] = 0;

    StackNode stack[1000];
    i32 stack_count = 0;
    stack[stack_count++] = (StackNode){ start, (v2){ 1, 0 } };
    while (stack_count > 0) {
        StackNode node = stack[--stack_count];
        v2 position = node.position;
        v2 direction = node.direction;
        i32 current_value = map[position.y*width+position.x];

        for (i32 d = 0; d < 4; d++) {
            if (v2_eq(directions[d], v2_flip(direction))) continue;

            v2 next_position = v2_add(position, directions[d]);
            i32 next_index = next_position.y*width+next_position.x;

            if (map[next_index] <= current_value) continue;

            map[next_index] = current_value + (v2_eq(directions[d], direction) ? 1 : 1001);
            stack[stack_count++] = (StackNode){ next_position, directions[d] };
        }
    }

    return map[end.y*width+end.x];
}

typedef struct { i8 type; i32 values[4]; } Tile;

u64 part_2(u8* content, u32 file_size) {
    Tile* tiles = (Tile*)malloc(file_size*sizeof(Tile));
    for (i32 i = 0; i < file_size; i++) {
        if (content[i] == 'S') {
            start.x = i % width;
            start.y = i / width;
        } else if (content[i] == 'E') {
            end.x = i % width;
            end.y = i / width;
        }

        tiles[i].type = (content[i] == '#') ? 0 : 1;
        tiles[i].values[0] = I32_MAX;
        tiles[i].values[1] = I32_MAX;
        tiles[i].values[2] = I32_MAX;
        tiles[i].values[3] = I32_MAX;
    }

    StackNode stack[1000];
    i32 stack_count = 0;
    stack[stack_count++] = (StackNode){ start, (v2){ 1, 0 }, 0 };
    while (stack_count > 0) {
        StackNode node = stack[--stack_count];
        v2 position = node.position;
        v2 direction = node.direction;

        for (i32 d = 0; d < 4; d++) {
            if (v2_eq(directions[d], v2_flip(direction))) continue;

            v2 next_position = v2_add(position, directions[d]);
            i32 next_index = next_position.y*width+next_position.x;
            if (tiles[next_index].type == 0) continue;

            i32 next_value = node.value + (v2_eq(directions[d], direction) ? 1 : 1001);
            if (tiles[next_index].values[d] > next_value) {
                tiles[next_index].values[d] = next_value;
                stack[stack_count++] = (StackNode){ next_position, directions[d], next_value };
            }
        }
    }

    stack[stack_count++] = (StackNode){ end };
    while (stack_count > 0) {
        StackNode node = stack[--stack_count];
        v2 position = node.position;
        v2 direction = node.direction;

        Tile* tile = &tiles[position.y*width+position.x];
        tile->type = 2;
        if (v2_eq(position, start)) continue;

        i32 lowest_value = I32_MAX;
        for (i32 d = 0; d < 4; d++) {
            if (tile->values[d] < lowest_value) {
                lowest_value = tile->values[d];
            }
        }

        for (i32 d = 0; d < 4; d++) {
            i32 next_value = tile->values[d];
            v2 dir = directions[(d+2)%4];

            if (next_value == lowest_value || (v2_eq(dir, direction) && next_value == lowest_value+1000)) {
                v2 next_position = v2_add(position, dir);
                i32 next_index = next_position.y*width+next_position.x;
                stack[stack_count++] = (StackNode){ next_position, dir };
            }
        }
    }

    i32 sum = 0;
    for (i32 i = 0; i < file_size; i++) {
        if (tiles[i].type == 2) sum++;
    }

    return sum;
}


i32 main() {
    u32 file_size;
    u8* content = load_file_wh("input16.txt", &file_size, &width, &height);
    printf("part 1: %llu\n", part_1(content, file_size));
    printf("part 2: %llu\n", part_2(content, file_size));

    return 0;
}
