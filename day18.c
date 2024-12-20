#include "basic.c"
#define WIDTH 71
#define HEIGHT 71

typedef struct { i32 x, y; } v2;
b8 v2_eq(v2 a, v2 b) { return (a.x == b.x && a.y == b.y); }
v2 v2_add(v2 a, v2 b) { return (v2){ a.x+b.x, a.y+b.y }; }

v2 directions[4] = {{0,-1},{1,0},{0,1},{-1,0}};
v2 start = {0,0};
v2 end = {WIDTH-1,HEIGHT-1};
u8 map[WIDTH*HEIGHT] = {};

typedef struct { v2 position; v2 parent; i32 a_dist, b_dist, f_cost; } Node;
Node open[WIDTH*HEIGHT];
Node closed[WIDTH*HEIGHT];
v2 path[WIDTH*HEIGHT];
i32 open_count = 0;
i32 closed_count = 0;
i32 path_count = 0;

b8 is_in_open_list(v2 p) {
    for (i32 i = 0; i < open_count; i++) if (v2_eq(p, open[i].position)) return true;
    return false;
}
b8 is_in_closed_list(v2 p) {
    for (i32 i = 0; i < closed_count; i++) if (v2_eq(p, closed[i].position)) return true;
    return false;
}
i32 manhattan_distance(v2 a, v2 b) {
    return abs(a.x - b.x) + abs(a.y - b.y);
}

i32 a_star() {
    open_count = 0;
    closed_count = 0;
    path_count = 0;
    open[open_count++] = (Node){ start, {-1,-1}, 0, manhattan_distance(start, end), manhattan_distance(start, end)};

    i32 steps = 0;
    while (open_count > 0) {
        i32 current_index = 0;
        for (int i = 1; i < open_count; i++) {
            if (open[i].f_cost < open[current_index].f_cost) current_index = i;
        }
        Node current = open[current_index];

        open[current_index] = open[--open_count];
        closed[closed_count++] = current;

        if (v2_eq(current.position, end)) {
            while (!v2_eq(current.position, start)) {
                for (i32 i = 0; i < closed_count; i++) {
                    if (v2_eq(closed[i].position, current.parent)) {
                        current = closed[i];
                        path[path_count++] = current.position;
                        steps++;
                        break;
                    }
                }
            }

            return steps;
        }

        for (int d = 0; d < 4; d++) {
            v2 neighbor = v2_add(current.position, directions[d]);

            if      (neighbor.x < 0 || neighbor.x >= WIDTH) continue;
            else if (neighbor.y < 0 || neighbor.y >= HEIGHT) continue;
            else if (map[neighbor.y*WIDTH+neighbor.x] == 1) continue;
            else if (is_in_closed_list(neighbor)) continue;

            int new_a_dist = current.a_dist + 1;

            if (!is_in_open_list(neighbor)) {
                Node node = {};
                node.position = neighbor;
                node.parent = current.position;
                node.a_dist = new_a_dist;
                node.b_dist = manhattan_distance(neighbor, end);
                node.f_cost = node.a_dist + node.b_dist;
                open[open_count++] = node;
            } else {
                for (int j = 0; j < open_count; j++) {
                    if (v2_eq(open[j].position, neighbor) && new_a_dist < open[j].a_dist) {
                        open[j].a_dist = new_a_dist;
                        open[j].f_cost = open[j].a_dist + open[j].b_dist;
                        open[j].parent = current.position;
                    }
                }
            }
        }
    }

    return -1;
}

void part_1(u8* content, u32 file_size) {
    i32 n = 0;
    i32 x = -1;
    i32 fallen_bytes = 0;
    for (i32 i = 0; i < file_size; i++) {
        if (content[i] >= '0' && content[i] <= '9') {
            n *= 10;
            n += content[i]-'0';
        } else if (content[i] == ',' || content[i] == '\n') {
            if (x == -1) {
                x = n;
            } else {
                map[n*WIDTH+x] = 1;
                x = -1;
                if (++fallen_bytes == 1024) break;
            }
            n = 0;
        }
    }

    printf("part 1: %i\n", a_star());
}

void part_2(u8* content, u32 file_size) {
    i32 n = 0;
    i32 x = -1;
    i32 fallen_bytes = 0;
    a_star();
    for (i32 i = 0; i < file_size; i++) {
        if (content[i] >= '0' && content[i] <= '9') {
            n *= 10;
            n += content[i]-'0';
        } else if (content[i] == ',' || content[i] == '\n') {
            if (x == -1) {
                x = n;
            } else {
                map[n*WIDTH+x] = 1;
                b8 blocked_current_path = false;
                for (i32 p = 0; p < path_count; p++) {
                    if (v2_eq(path[p], (v2){ x, n })) {
                        blocked_current_path = true;
                        break;
                    }
                }
                if (blocked_current_path && a_star() == -1) {
                    printf("part 2: %i,%i\n", x, n);
                    break;
                }
                x = -1;
            }
            n = 0;
        }
    }
}

i32 main() {
    u32 file_size;
    u8* content = load_file("input18.txt", &file_size);
    part_1(content, file_size);
    part_2(content, file_size);

    return 0;
}
