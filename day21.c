#include "basic.c"
#include <string.h>

typedef struct { i32 x, y; } v2;
b8 v2_eq(v2 a, v2 b) { return (a.x == b.x && a.y == b.y); }
v2 v2_add(v2 a, v2 b) { return (v2){ a.x+b.x, a.y+b.y }; }
v2 v2_sub(v2 a, v2 b) { return (v2){ a.x-b.x, a.y-b.y }; }
v2 keypad_coords[11] = {{1,3},{0,2},{1,2},{2,2},{0,1},{1,1},{2,1},{0,0},{1,0},{2,0},{2,3}};
v2 joypad_coords[5] = {{1,0},{2,1},{1,1},{0,1},{2,0}};
v2 keypad(u8 c) {
    if (c == 'A') return keypad_coords[10];
    if (c == ' ') return (v2){ 0, 3 };
    return keypad_coords[c-'0'];
}
v2 joypad(u8 c) {
    if (c == ' ') return (v2){ 0, 0 };
    if (c == '^') return joypad_coords[0];
    if (c == '>') return joypad_coords[1];
    if (c == 'v') return joypad_coords[2];
    if (c == '<') return joypad_coords[3];
    return joypad_coords[4];
}

u64 part_1(u8* content, u32 file_size) {
    v2 robot[3] = { keypad('A'), joypad('A'), joypad('A') };

    u8* code = (u8*)malloc(1000);
    u8* code_new = (u8*)malloc(1000);
    i32 code_count = 0;
    i32 code_new_count = 0;

    i32 n = 0;
    i32 sum = 0;
    for (i32 i = 0; i < file_size; i++) {
        if (content[i] != '\n') {
            code[code_count++] = content[i];
            if (content[i] != 'A') {
                n *= 10;
                n += content[i]-'0';
            }
        } else {
            for (i32 r = 0; r < 3; r++) {
                for (i32 c = 0; c < code_count; c++) {
                    v2 coord = (r == 0) ? keypad(code[c]) : joypad(code[c]);
                    v2 gap = (r == 0) ? (v2){0,3} : (v2){0,0};
                    v2 dt = v2_sub(coord, robot[r]);
                    b8 x_issue = robot[r].y == gap.y && robot[r].x+dt.x == gap.x;
                    b8 y_issue = robot[r].x == gap.x && robot[r].y+dt.y == gap.y;

                    i32 x = dt.x;
                    i32 y = dt.y;
                    if (!x_issue) for (; x < 0; x++) code_new[code_new_count++] = '<';

                    if (!y_issue) for (; y < 0; y++) code_new[code_new_count++] = '^';
                    if (!y_issue) for (; y > 0; y--) code_new[code_new_count++] = 'v';

                    for (; x < 0; x++) code_new[code_new_count++] = '<';
                    for (; x > 0; x--) code_new[code_new_count++] = '>';

                    for (; y < 0; y++) code_new[code_new_count++] = '^';
                    for (; y > 0; y--) code_new[code_new_count++] = 'v';

                    code_new[code_new_count++] = 'A';
                    robot[r] = coord;
                }

                u8* tmp = code;
                code = code_new;
                code_new = tmp;
                code_count = code_new_count;
                code_new_count = 0;

                code[code_count] = 0;
            }

            sum += n * code_count;
            n = 0;
            code_count = 0;
            code_new_count = 0;
            robot[0] = keypad('A');
            robot[1] = joypad('A');
            robot[2] = joypad('A');
        }
    }

    return sum;
}

typedef struct {
    char* key;
    i64 value;
} CacheItem;

CacheItem* cache;
i32 cache_count = 0;

CacheItem* get_cache(char* key) {
    for (i32 i = 0; i < cache_count; i++) {
        if (!strcmp(cache[i].key, key)) return &cache[i];
    }
    return NULL;
}

i32 d2i(u8 c) {
    if (c == '^') return 0;
    if (c == 'v') return 1;
    if (c == '<') return 2;
    if (c == '>') return 3;
    if (c == 'A') return 4;
}
i32 k2i(u8 c) {
    if (c == 'A') return 10;
    return c-'0';
}

char* joypad_lookup[5][5] = {};
char* keypad_lookup[11][11][2] = {};

i64 find_length(u8* token, i32 steps_left) {
    if (steps_left == 0) { return strlen(token); }

    char steps_left_char[10];
    snprintf(steps_left_char, 10, "%i", steps_left);
    i32 id_length = strlen(token) + 1 + strlen(steps_left_char) + 1;
    char* id = (char*)malloc(id_length);
    snprintf(id, id_length, "%s|%s", token, steps_left_char);

    CacheItem* item = get_cache(id);
    if (item != NULL) { return item->value; }

    i64 length = 0;
    char last_button = 'A';
    for (i32 i = 0; i < strlen(token); i++) {
        char* new_token = joypad_lookup[d2i(last_button)][d2i(token[i])];
        last_button = token[i];
        length += find_length(new_token, steps_left-1);
    }

    cache[cache_count++] = (CacheItem){ id, length };
    return length;
}

i64 complexity(u8* code, i32 code_count) {
    char** sequences     = (char**)malloc(1000 * sizeof(char*));
    char** sequences_new = (char**)malloc(1000 * sizeof(char*));

    char* empty = (char*)malloc(1);
    empty = "";
    sequences[0] = empty;
    i32 sequences_count = 1;
    i32 sequences_new_count = 0;

    u8 last_button = 'A';
    for (i32 i = 0; i < code_count; i++) {
        char* commands[2] = {
            keypad_lookup[k2i(last_button)][k2i(code[i])][0],
            keypad_lookup[k2i(last_button)][k2i(code[i])][1]
        };
        last_button = code[i];

        sequences_new_count = 0;
        for (i32 c = 0; c < 2; c++) {
            if (!commands[c]) continue;
            for (i32 s = 0; s < sequences_count; s++) {
                i32 new_length = strlen(sequences[s]) + strlen(commands[c]);
                char* sequence = (char*)malloc(new_length);
                memcpy(sequence, sequences[s], strlen(sequences[s]));
                memcpy(sequence + strlen(sequences[s]), commands[c], strlen(commands[c]));
                sequences_new[sequences_new_count++] = sequence;
            }
        }

        char** tmp = sequences;
        sequences = sequences_new;
        sequences_new = tmp;

        sequences_count = sequences_new_count;
    }

    i64 min_length = 9223372036854775807;
    for (i32 i = 0; i < sequences_count; i++) {
        i32 sequence_length = strlen(sequences[i]);

        i64 length = 0;
        char token[1024] = {};
        i32 token_length = 0;
        for (i32 j = 0; j < sequence_length; j += token_length) {
            for (i32 k = 0; k < sequence_length; k++) {
                token[k] = sequences[i][j+k];
                if (sequences[i][j+k] == 'A') { token_length = k; break; }
            }
            token[token_length++] = 'A';
            token[token_length] = 0;
            i64 l = find_length(token, 25);
            length += l;
        }

        if (length < min_length) min_length = length;
    }


    return min_length;
}

u64 part_2(u8* content, u32 file_size) {
    joypad_lookup[0][0] = "A";
    joypad_lookup[0][1] = "vA";
    joypad_lookup[0][2] = "v<A";
    joypad_lookup[0][3] = "v>A";
    joypad_lookup[0][4] = ">A";

    joypad_lookup[1][0] = "^A";
    joypad_lookup[1][1] = "A";
    joypad_lookup[1][2] = "<A";
    joypad_lookup[1][3] = ">A";
    joypad_lookup[1][4] = "^>A";

    joypad_lookup[2][0] = ">^A";
    joypad_lookup[2][1] = ">A";
    joypad_lookup[2][2] = "A";
    joypad_lookup[2][3] = ">>A";
    joypad_lookup[2][4] = ">>^A";

    joypad_lookup[3][0] = "<^A";
    joypad_lookup[3][1] = "<A";
    joypad_lookup[3][2] = "<<A";
    joypad_lookup[3][3] = "A";
    joypad_lookup[3][4] = "^A";

    joypad_lookup[4][0] = "<A";
    joypad_lookup[4][1] = "<vA";
    joypad_lookup[4][2] = "v<<A";
    joypad_lookup[4][3] = "vA";
    joypad_lookup[4][4] = "A";

    keypad_lookup[0][0][0] = "A";
    keypad_lookup[0][1][0] = "^<A";
    keypad_lookup[0][2][0] = "^A";
    keypad_lookup[0][3][0] = "^>A";
    keypad_lookup[0][3][1] = ">^A";
    keypad_lookup[0][4][0] = "^^<A";
    keypad_lookup[0][5][0] = "^^A";
    keypad_lookup[0][6][0] = "^^>A";
    keypad_lookup[0][6][1] = ">^^A";
    keypad_lookup[0][7][0] = "^^^<A";
    keypad_lookup[0][8][0] = "^^^A";
    keypad_lookup[0][9][0] = "^^^>A";
    keypad_lookup[0][9][1] = ">^^^A";
    keypad_lookup[0][10][0] = ">A";

    keypad_lookup[1][0][0] = ">vA";
    keypad_lookup[1][1][0] = "A";
    keypad_lookup[1][2][0] = ">A";
    keypad_lookup[1][3][0] = ">>A";
    keypad_lookup[1][4][0] = "^A";
    keypad_lookup[1][5][0] = "^>A";
    keypad_lookup[1][5][1] = ">^A";
    keypad_lookup[1][6][0] = "^>>A";
    keypad_lookup[1][6][1] = ">>^A";
    keypad_lookup[1][7][0] = "^^A";
    keypad_lookup[1][8][0] = "^^>A";
    keypad_lookup[1][8][1] = ">^^A";
    keypad_lookup[1][9][0] = "^^>>A";
    keypad_lookup[1][9][1] = ">>^^A";
    keypad_lookup[1][10][0] = ">>vA";

    keypad_lookup[2][0][0] = "vA";
    keypad_lookup[2][1][0] = "<A";
    keypad_lookup[2][2][0] = "A";
    keypad_lookup[2][3][0] = ">A";
    keypad_lookup[2][4][0] = "^<A";
    keypad_lookup[2][4][1] = "<^A";
    keypad_lookup[2][5][0] = "^A";
    keypad_lookup[2][6][0] = "^>A";
    keypad_lookup[2][6][1] = ">^A";
    keypad_lookup[2][7][0] = "^^<A";
    keypad_lookup[2][7][1] = "<^^A";
    keypad_lookup[2][8][0] = "^^A";
    keypad_lookup[2][9][0] = "^^>A";
    keypad_lookup[2][9][1] = ">^^A";
    keypad_lookup[2][10][0] = "v>A";
    keypad_lookup[2][10][1] = ">vA";

    keypad_lookup[3][0][0] = "v<A";
    keypad_lookup[3][0][1] = "<vA";
    keypad_lookup[3][1][0] = "<<A";
    keypad_lookup[3][2][0] = "<A";
    keypad_lookup[3][3][0] = "A";
    keypad_lookup[3][4][0] = "^<<A";
    keypad_lookup[3][4][1] = "<<^A";
    keypad_lookup[3][5][0] = "^<A";
    keypad_lookup[3][5][1] = "<^A";
    keypad_lookup[3][6][0] = "^A";
    keypad_lookup[3][7][0] = "^^<<A";
    keypad_lookup[3][7][1] = "<<^^A";
    keypad_lookup[3][8][0] = "^^<A";
    keypad_lookup[3][8][1] = "<^^A";
    keypad_lookup[3][9][0] = "^^A";
    keypad_lookup[3][10][0] = "vA";

    keypad_lookup[4][0][0] = ">vvA";
    keypad_lookup[4][1][0] = "vA";
    keypad_lookup[4][2][0] = "v>A";
    keypad_lookup[4][2][1] = ">vA";
    keypad_lookup[4][3][0] = "v>>A";
    keypad_lookup[4][3][1] = ">>vA";
    keypad_lookup[4][4][0] = "A";
    keypad_lookup[4][5][0] = ">A";
    keypad_lookup[4][6][0] = ">>A";
    keypad_lookup[4][7][0] = "^A";
    keypad_lookup[4][8][0] = "^>A";
    keypad_lookup[4][8][1] = ">^A";
    keypad_lookup[4][9][0] = "^>>A";
    keypad_lookup[4][9][1] = ">>^A";
    keypad_lookup[4][10][0] = ">>vvA";

    keypad_lookup[5][0][0] = "vvA";
    keypad_lookup[5][1][0] = "v<A";
    keypad_lookup[5][1][1] = "<vA";
    keypad_lookup[5][2][0] = "vA";
    keypad_lookup[5][3][0] = "v>A";
    keypad_lookup[5][3][1] = ">vA";
    keypad_lookup[5][4][0] = "<A";
    keypad_lookup[5][5][0] = "A";
    keypad_lookup[5][6][0] = ">A";
    keypad_lookup[5][7][0] = "^<A";
    keypad_lookup[5][7][1] = "<^A";
    keypad_lookup[5][8][0] = "^A";
    keypad_lookup[5][9][0] = "^>A";
    keypad_lookup[5][9][1] = ">^A";
    keypad_lookup[5][10][0] = "vv>A";
    keypad_lookup[5][10][1] = ">vvA";

    keypad_lookup[6][0][0] = "vv<A";
    keypad_lookup[6][0][1] = "<vvA";
    keypad_lookup[6][1][0] = "v<<A";
    keypad_lookup[6][1][1] = "<<vA";
    keypad_lookup[6][2][0] = "v<A";
    keypad_lookup[6][2][1] = "<vA";
    keypad_lookup[6][3][0] = "vA";
    keypad_lookup[6][4][0] = "<<A";
    keypad_lookup[6][5][0] = "<A";
    keypad_lookup[6][6][0] = "A";
    keypad_lookup[6][7][0] = "^<<A";
    keypad_lookup[6][7][1] = "<<^A";
    keypad_lookup[6][8][0] = "^<A";
    keypad_lookup[6][8][1] = "<^A";
    keypad_lookup[6][9][0] = "^A";
    keypad_lookup[6][10][0] = "vvA";

    keypad_lookup[7][0][0] = ">vvvA";
    keypad_lookup[7][1][0] = "vvA";
    keypad_lookup[7][2][0] = "vv>A";
    keypad_lookup[7][2][1] = ">vvA";
    keypad_lookup[7][3][0] = "vv>>A";
    keypad_lookup[7][3][1] = ">>vvA";
    keypad_lookup[7][4][0] = "vA";
    keypad_lookup[7][5][0] = "v>A";
    keypad_lookup[7][5][1] = ">vA";
    keypad_lookup[7][6][0] = "v>>A";
    keypad_lookup[7][6][1] = ">>vA";
    keypad_lookup[7][7][0] = "A";
    keypad_lookup[7][8][0] = ">A";
    keypad_lookup[7][9][0] = ">>A";
    keypad_lookup[7][10][0] = ">>vvvA";

    keypad_lookup[8][0][0] = "vvvA";
    keypad_lookup[8][1][0] = "vv<A";
    keypad_lookup[8][1][1] = "<vvA";
    keypad_lookup[8][2][0] = "vvA";
    keypad_lookup[8][3][0] = "vv>A";
    keypad_lookup[8][3][1] = ">vvA";
    keypad_lookup[8][4][0] = "v<A";
    keypad_lookup[8][4][1] = "<vA";
    keypad_lookup[8][5][0] = "vA";
    keypad_lookup[8][6][0] = "v>A";
    keypad_lookup[8][6][1] = ">vA";
    keypad_lookup[8][7][0] = "<A";
    keypad_lookup[8][8][0] = "A";
    keypad_lookup[8][9][0] = ">A";
    keypad_lookup[8][10][0] = "vvv>A";
    keypad_lookup[8][10][1] = ">vvvA";

    keypad_lookup[9][0][0] = "vvv<A";
    keypad_lookup[9][0][1] = "<vvvA";
    keypad_lookup[9][1][0] = "vv<<A";
    keypad_lookup[9][1][1] = "<<vvA";
    keypad_lookup[9][2][0] = "vv<A";
    keypad_lookup[9][2][1] = "<vvA";
    keypad_lookup[9][3][0] = "vvA";
    keypad_lookup[9][4][0] = "v<<A";
    keypad_lookup[9][4][1] = "<<vA";
    keypad_lookup[9][5][0] = "v<A";
    keypad_lookup[9][5][1] = "<vA";
    keypad_lookup[9][6][0] = "vA";
    keypad_lookup[9][7][0] = "<<A";
    keypad_lookup[9][8][0] = "<A";
    keypad_lookup[9][9][0] = "A";
    keypad_lookup[9][10][0] = "vvvA";

    keypad_lookup[10][0][0] = "<A";
    keypad_lookup[10][1][0] = "^<<A";
    keypad_lookup[10][2][0] = "^<A";
    keypad_lookup[10][2][1] = "<^A";
    keypad_lookup[10][3][0] = "^A";
    keypad_lookup[10][4][0] = "^^<<A";
    keypad_lookup[10][5][0] = "^^<A";
    keypad_lookup[10][5][1] = "<^^A";
    keypad_lookup[10][6][0] = "^^A";
    keypad_lookup[10][7][0] = "^^^<<A";
    keypad_lookup[10][8][0] = "^^^<A";
    keypad_lookup[10][8][1] = "<^^^A";
    keypad_lookup[10][9][0] = "^^^A";
    keypad_lookup[10][10][0] = "A";

    cache = (CacheItem*)malloc(10000 * sizeof(CacheItem));
    u8* code = (u8*)malloc(1000);
    u8* code_new = (u8*)malloc(1000);
    i32 code_count = 0;
    i32 code_new_count = 0;

    i32 n = 0;
    i64 sum = 0;
    for (i32 i = 0; i < file_size; i++) {
        if (content[i] != '\n') {
            code[code_count++] = content[i];
            if (content[i] != 'A') {
                n *= 10;
                n += content[i]-'0';
            }
        } else {
            i64 result = n * complexity(code, code_count);
            sum += result;
            n = 0;
            code_count = 0;
        }
    }

    return sum;
}

i32 main() {
    u32 file_size;
    u8* content = load_file("input21.txt", &file_size);
    printf("part 1: %llu\n", part_1(content, file_size));
    printf("part 2: %llu\n", part_2(content, file_size));

    return 0;
}
