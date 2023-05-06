// This test shows how existential types can be used in combination
// with generic functions and structs to create type-safe callback library
// and client code.
//
// RUN: %clang %s -o %t1 %checkedc_target_flags

// RUN: %checkedc_rununder %t1 -DARR | FileCheck %s --check-prefix TEST


#include <stdlib.h>
#include <stdio.h>

// Library code

#define MAX_CB 100

typedef int event_id;

#define EVENT_START 0
#define EVENT_PAUSE 1
#define EVENT_STOP 2

struct cb_info _For_any(T) {
    T *data;
    void (*cb)(event_id id, T *data);
};

struct map_entry {
    event_id id;
    _Exists(T, struct cb_info<T>) info;
};

int num_entries = 0;
struct map_entry cb_map[MAX_CB];

_For_any(T) void reg_cb(event_id id, struct cb_info<T> info) {
    _Exists(A, struct cb_info<A>) opaque_info = _Pack(info, _Exists(A, struct cb_info<A>), T);
    struct map_entry entry;
    entry.id = id;
    entry.info = opaque_info;
    cb_map[num_entries] = entry;
    num_entries += 1;
}

void handle(event_id id) {
    for (int i = 0; i < num_entries; i++) {
        if (cb_map[i].id == id) {
            _Unpack (T) struct cb_info<T> cb_inf = cb_map[i].info;
            cb_inf.cb(id, cb_inf.data);
        }
    }
}

// User code
void cb_start(event_id id, char *task_name) {
    printf("cb_start: id = %d, task_name = %s\n", id, task_name);
}

void cb_pause(event_id id, int *resume_in) {
    printf("cb_pause: id = %d, resume_in = %ds\n", id, *resume_in);
}

struct continue_info {
    int follow_by;
};

void cb_stop(event_id id, struct continue_info *info) {
    printf("cb_stop: id = %d, follow_by = %d\n", id, info->follow_by);
}

void client_register() {
    struct cb_info<char> start_info = { "task1", &cb_start };
    struct cb_info<char> start_info2 = { "task2", &cb_start };

    // Unfortunately, we need to malloc the client-passed arguments to the callback.
    // This is because type parameters can only appear as pointers inside generic structs.
    // Eventually, we relax this constraint.
    int *resume_in = (int *) malloc(sizeof(int));
    *resume_in = 42;
    struct cb_info<int> pause_info = { resume_in, &cb_pause };

    struct continue_info *cont_info = (struct continue_info *) malloc(sizeof(struct continue_info));
    cont_info->follow_by = 100;
    struct cb_info<struct continue_info> stop_info = { cont_info, &cb_stop };

    reg_cb<char>(EVENT_START, start_info);
    reg_cb<char>(EVENT_START, start_info2);
    reg_cb<int>(EVENT_PAUSE, pause_info);
    reg_cb<struct continue_info>(EVENT_STOP, stop_info);
}

int main(int argc, char *argv[]) {
    client_register();
    handle(EVENT_START);
    // TEST: cb_start: id = 0, task_name = task1
    // TEST: cb_start: id = 0, task_name = task2

    handle(EVENT_PAUSE);
    // TEST: cb_pause: id = 1, resume_in = 42s

    handle(EVENT_STOP);
    // TEST: cb_stop: id = 2, follow_by = 100

    handle(EVENT_PAUSE);
    // TEST: cb_pause: id = 1, resume_in = 42s
    handle(EVENT_PAUSE);
    // TEST: cb_pause: id = 1, resume_in = 42s
}


