#include "basic.c"
#include <string.h>

typedef struct { i16 connections[676]; i32 connections_count; } Computer;
Computer computers[676] = {};
void remove_connection(i16 id, i16 remove) {
    for (i32 x = 0; x < computers[id].connections_count; x++) {
        if (computers[id].connections[x] == remove) {
            computers[id].connections[x] = -1;
            break;
        }
    }

    for (i32 x = 0; x < computers[remove].connections_count; x++) {
        if (computers[remove].connections[x] == id) {
            computers[remove].connections[x] = -1;
            break;
        }
    }
}
u64 find_groups(i16 id, i16 start_id, i16 previous_id, i32 steps) {
    if (steps >= 3) return 0;

    Computer* computer = &computers[id];
    i32 loops = 0;
    for (i32 i = 0; i < computer->connections_count; i++) {
        i16 cid = computer->connections[i];
        if (cid == -1) continue;
        if (cid == previous_id) continue;
        if (cid == start_id) {
            remove_connection(start_id, previous_id);
            if ((start_id / 26) == 't'-'a' || (previous_id / 26) == 't'-'a' || (id / 26) == 't'-'a')
            {
                return 1;
            }
        }
        loops += find_groups(cid, start_id, id, steps+1);
    }

    return loops;
}
u64 part_1(u8* content, u32 file_size) {
    i32 chars[4] = {};
    i32 chars_count = 0;
    for (i32 i = 0; i < file_size; i++) {
        if (content[i] == '-') { }
        else if (content[i] != '\n') {
            chars[chars_count++] = content[i];
        } else {
            u16 a_index = 26*(chars[0]-'a') + (chars[1]-'a');
            u16 b_index = 26*(chars[2]-'a') + (chars[3]-'a');
            computers[a_index].connections[computers[a_index].connections_count++] = b_index;
            computers[b_index].connections[computers[b_index].connections_count++] = a_index;
            chars_count = 0;
        }
    }

    i32 sum = 0;
    for (i32 i = 0; i < 676; i++) {
        sum += find_groups(i, i, i, 0);
    }

    return sum;
}

i32 compare(const void* a, const void* b) {
   return (*(i16*)a - *(i16*)b);
}

u64 part_2(u8* content, u32 file_size) {
    memset(computers, 0, sizeof(computers));
    i32 chars[4] = {};
    i32 chars_count = 0;
    for (i32 i = 0; i < file_size; i++) {
        if (content[i] == '-') { }
        else if (content[i] != '\n') {
            chars[chars_count++] = content[i];
        } else {
            u16 a_index = 26*(chars[0]-'a') + (chars[1]-'a');
            u16 b_index = 26*(chars[2]-'a') + (chars[3]-'a');
            computers[a_index].connections[computers[a_index].connections_count++] = b_index;
            computers[b_index].connections[computers[b_index].connections_count++] = a_index;
            chars_count = 0;
        }
    }

    Computer largest_network = {};
    for (i32 i = 0; i < 676; i++) {
        if (!computers[i].connections_count) continue;
        Computer network = computers[i];
        network.connections_count--;

        b8 is_all_interconnected = true;
        for (i32 a = 0; a < network.connections_count-1; a++) {
            i16 computer_a = network.connections[a];

            for (i32 b = a+1; b < network.connections_count; b++) {
                i16 computer_b = network.connections[b];

                b8 includes = false;
                for (i32 c = 0; c < computers[computer_a].connections_count; c++) {
                    if (computers[computer_a].connections[c] == computer_b) { includes = true; break; }
                }
                if (!includes) { is_all_interconnected = false; break; }
            }
            if (!is_all_interconnected) { break; }
        }
        if (!is_all_interconnected) { continue; }

        if (network.connections_count > largest_network.connections_count) {
            network.connections[network.connections_count++] = i;
            largest_network = network;
        }
    }

    qsort(largest_network.connections, largest_network.connections_count, sizeof(i16), compare);
    for (i32 i = 0; i < largest_network.connections_count; i++) {
        i16 c = largest_network.connections[i];
        printf("%c%c,", (c/26)+'a', (c%26)+'a');
    }

    return 0;

    printf("\n");
}

i32 main() {
    u32 file_size;
    u8* content = load_file("input23.txt", &file_size);
    printf("part 1: %llu\n", part_1(content, file_size));
    printf("part 2: "); part_2(content, file_size);

    return 0;
}
