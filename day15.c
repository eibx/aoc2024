#include "basic.c"
i32 width = 0;
i32 height = 0;

#define MAP_STATE 0
#define INSTRUCTION_STATE 1

typedef struct { i32 x, y; } v2;
v2 flip(v2 a) { return (v2){ a.x*-1, a.y*-1 }; }
v2 add(v2 a, v2 b) { return (v2){ a.x+b.x, a.y+b.y }; }
u8 tile(u8* content, v2 p) { return content[p.y*width+p.x]; }
u64 part_1(u8* content, u32 file_size) {
    b8 state = MAP_STATE;
    v2 position = {0};
    for (i32 i = 0; i < file_size; i++) {
        if (state == MAP_STATE) {
            if (content[i] == '\n') {
                if (width == 0) width = i+1;
                if (content[i+1] == '\n') {
                    height = i / width;
                    state = INSTRUCTION_STATE;
                    i++;
                }
            } else if (content[i] == '@') {
                position.x = i % (width);
                position.y = i / (width);
            }
        } else {
            v2 direction = {0};
            if      (content[i] == '^') { direction = (v2){ 0, -1 }; }
            else if (content[i] == '>') { direction = (v2){ +1, 0 }; }
            else if (content[i] == 'v') { direction = (v2){ 0, +1 }; }
            else if (content[i] == '<') { direction = (v2){ -1, 0 }; }

            v2 next_position = add(position, direction);
            u8 next_tile = tile(content, next_position);

            if (next_tile == '.') {
                content[position.y*width+position.x] = '.';
                content[next_position.y*width+next_position.x] = '@';
                position = next_position;
            } else if (next_tile == 'O') {
                b8 can_move = false;
                v2 tmp = next_position;
                while (true) {
                    tmp = add(tmp, direction);
                    u8 tmp_tile = tile(content, tmp);
                    if (tmp_tile == '.') {
                        can_move = true;
                        break;
                    } else if (tmp_tile == '#') {
                        break;
                    }
                }

                if (can_move) {
                    v2 back_direction = flip(direction);
                    while (true) {
                        b8 is_player_tile = tile(content, tmp) == '@';
                        if (!is_player_tile) {
                            v2 new_tmp = add(tmp, back_direction);
                            content[tmp.y*width+tmp.x] = content[new_tmp.y*width+new_tmp.x];
                            tmp = new_tmp;
                        } else {
                            content[tmp.y*width+tmp.x] = '.';
                            position = next_position;
                            break;
                        }
                    }
                }
            }
        }
    }

    i32 sum = 0;
    for (i32 i = 0; i < file_size; i++) {
        if (content[i] == 'O') {
            sum += 100*(i/width)+i%width;
        }
    }

    return sum;
}

u64 part_2(u8* content, u32 file_size) {
    b8 state = MAP_STATE;
    v2 position = {0};
    u8* map;
    width = 0;
    for (i32 i = 0; i < file_size; i++) {
        if (state == MAP_STATE) {
            if (content[i] == '\n') {
                if (width == 0) width = i+1;
                if (content[i+1] == '\n') {
                    height = (i+1) / width;
                    state = INSTRUCTION_STATE;

                    map = (u8*)malloc(width*2*height);
                    i32 map_index = 0;

                    for (i32 y = 0; y < height; y++) {
                        for (i32 x = 0; x < width; x++) {
                            u8 tile = content[y*width+x];
                            if (tile == '\n') continue;

                            u8 left, right;
                            if      (tile == '#') { left = '#'; right = '#'; }
                            else if (tile == '.') { left = '.'; right = '.'; }
                            else if (tile == 'O') { left = '['; right = ']'; }
                            else if (tile == '@') { left = '@'; right = '.'; }
                            map[map_index++] = left;
                            map[map_index++] = right;
                        }
                    }
                    position.x *= 2;
                    width = width*2-2;
                    i++;
                }
            } else if (content[i] == '@') {
                position.x = i % width;
                position.y = i / width;
            }
        } else {
            v2 direction = {0};
            if      (content[i] == '^') { direction = (v2){ 0, -1 }; }
            else if (content[i] == '>') { direction = (v2){ +1, 0 }; }
            else if (content[i] == 'v') { direction = (v2){ 0, +1 }; }
            else if (content[i] == '<') { direction = (v2){ -1, 0 }; }

            v2 next_position = add(position, direction);
            u8 next_tile = map[next_position.y*width+next_position.x];

            if (next_tile == '.') {
                map[position.y*width+position.x] = '.';
                map[next_position.y*width+next_position.x] = '@';
                position = next_position;
            } else if (next_tile == '[' || next_tile == ']') {
                if (direction.y != 0) {
                    v2 crate_position = next_position;
                    if (next_tile == ']') crate_position.x--;

                    v2 crates[100000] = {0};
                    i32 crate_count = 0;
                    v2 crate_stack[100000] = {0};
                    i32 crate_stack_count = 0;
                    b8 can_move = true;
                    crate_stack[crate_stack_count++] = crate_position;
                    while (crate_stack_count > 0) {
                        v2 crate_left = crate_stack[--crate_stack_count];
                        crates[crate_count++] = crate_left;
                        v2 crate_right = (v2){ crate_left.x + 1, crate_left.y };

                        v2 crate_left_new = add(crate_left, direction);
                        v2 crate_right_new = add(crate_right, direction);

                        u8 crate_left_new_tile = map[crate_left_new.y*width+crate_left_new.x];
                        u8 crate_right_new_tile = map[crate_right_new.y*width+crate_right_new.x];

                        if (crate_left_new_tile == '#' || crate_right_new_tile == '#') {
                            can_move = false;
                            break;
                        }

                        if (crate_left_new_tile == '[') {
                            crate_stack[crate_stack_count++] = crate_left_new;
                        } else if (crate_left_new_tile == ']') {
                            v2 cc = (v2){ crate_left_new.x-1, crate_left_new.y };
                            crate_stack[crate_stack_count++] = cc;
                        }

                        if (crate_right_new_tile == '[') {
                            crate_stack[crate_stack_count++] = crate_right_new;
                        }
                    }

                    if (can_move) {
                        for (i32 i = crate_count-1; i >= 0; i--) {
                            v2 p = crates[i];
                            map[p.y*width+p.x+0] = '.';
                            map[p.y*width+p.x+1] = '.';
                        }
                        for (i32 i = crate_count-1; i >= 0; i--) {
                            v2 p = add(crates[i], direction);
                            map[p.y*width+p.x+0] = '[';
                            map[p.y*width+p.x+1] = ']';
                        }
                        map[position.y*width+position.x] = '.';
                        position = next_position;
                        map[position.y*width+position.x] = '@';
                    }
                } else {
                    v2 ch = next_position;
                    b8 can_move = true;
                    while (true) {
                        ch.x += direction.x;
                        u8 ch_tile = map[ch.y*width+ch.x];

                        if (ch_tile == '.') {
                            while (ch.x != position.x) {
                                map[ch.y*width+ch.x] = map[ch.y*width+ch.x + direction.x*-1];
                                ch.x += direction.x*-1;
                            }
                            break;
                        }

                        if (ch_tile == '#') {
                            can_move = false;
                            break;
                        }
                    }

                    if (can_move) {
                        map[position.y*width+position.x] = '.';
                        position = next_position;
                        map[position.y*width+position.x] = '@';
                    }
                }

            }
        }
    }

    i32 sum = 0;
    for (i32 y = 0; y < height; y++) {
        for (i32 x = 0; x < width; x++) {
            if (map[y*width+x] == '[') {
                sum += 100*y+x;
            }
        }
    }

    return sum;
}

i32 main() {
    u32 file_size;
    u8* content = load_file("input15.txt", &file_size);
    printf("part 1: %llu\n", part_1(content, file_size));
    content = load_file("input15.txt", &file_size);
    printf("part 2: %llu\n", part_2(content, file_size));

    return 0;
}
