#include "basic.c"
#define WIDTH 51
#define HEIGHT 50

typedef struct { i32 x, y; } v2;
v2 add(v2 a, v2 b) { return (v2){ a.x+b.x, a.y+b.y }; }
v2 sub(v2 a, v2 b) { return (v2){ a.x-b.x, a.y-b.y }; }

u64 part_1(u8* content, u32 file_size) {
    i32 sum = 0;
    u8 antinodes[WIDTH][HEIGHT] = {0};
    for (i32 i = 0; i < file_size; i++) {
        if (content[i] == '.' || content[i] == '\n') continue;

        v2 pos = (v2){ i % WIDTH, i / WIDTH };
        for (i32 j = 0; j < file_size; j++) {
            if (i == j || content[i] != content[j]) continue;

            v2 pos2 = (v2){ j % WIDTH, j / WIDTH };
            v2 manhattan = sub(pos2, pos);
            v2 antinode = add(pos2, manhattan);

            if (antinode.x < 0 || antinode.x >= WIDTH-1) continue;
            if (antinode.y < 0 || antinode.y >= HEIGHT) continue;

            if (antinodes[antinode.x][antinode.y] == 0) {
                antinodes[antinode.x][antinode.y] = 1;
                sum++;
            }
        }
    }

    return sum;
}

u64 part_2(u8* content, u32 file_size) {
    i32 sum = 0;
    u8 antinodes[WIDTH][HEIGHT] = {0};
    for (i32 i = 0; i < file_size; i++) {
        if (content[i] == '.' || content[i] == '\n') continue;

        v2 pos = (v2){ i % WIDTH, i / WIDTH };
        for (i32 j = 0; j < file_size; j++) {
            if (i == j || content[i] != content[j]) continue;

            v2 pos2 = (v2){ j % WIDTH, j / WIDTH };
            v2 manhattan = sub(pos2, pos);
            v2 antinode = pos2;

            if (antinodes[pos.x][pos.y] == 0) {
                antinodes[pos.x][pos.y] = 1;
                sum++;
            }
            if (antinodes[pos2.x][pos2.y] == 0) {
                antinodes[pos2.x][pos2.y] = 1;
                sum++;
            }

            while (true) {
                antinode = add(antinode, manhattan);

                if (antinode.x < 0 || antinode.x >= WIDTH-1) break;
                if (antinode.y < 0 || antinode.y >= HEIGHT) break;

                if (antinodes[antinode.x][antinode.y] == 0) {
                    antinodes[antinode.x][antinode.y] = 1;
                    sum++;
                }
            }
        }
    }

    return sum;
}

i32 main() {
    u32 file_size;
    u8* content = load_file("input08.txt", &file_size);
    printf("part 1: %llu\n", part_1(content, file_size));
    printf("part 2: %llu\n", part_2(content, file_size));

    return 0;
}
