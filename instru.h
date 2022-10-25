#ifndef INSTRU_H
#define INSTRU_H

/*
 * Performance Measurement & Instrumentation
 *
 * Copyright (c) 2022 b1f6c1c4 <b1f6c1c4@gmail.com>
 */

#include <time.h>

struct instru_t
{
	unsigned long total_duration;
	unsigned long num_invocations;

	struct timespec t1; // start of event
};

enum INSTRU_ID {
	INSTRU_main,
	INSTRU_cmd_clone,
	INSTRU_init_db,
	INSTRU_transport_get_remote_refs,
	INSTRU_NUM,
};

extern struct instru_t g_instrus[INSTRU_NUM];

inline __attribute__((always_inline)) void instru_t1(enum INSTRU_ID id)
{
	struct instru_t *obj = &g_instrus[id];
	clock_gettime(CLOCK_MONOTONIC, &obj->t1);
}

inline __attribute__((always_inline)) void instru_t2(enum INSTRU_ID id)
{
	struct timespec t2;
	clock_gettime(CLOCK_MONOTONIC, &t2);
	struct instru_t *obj = &g_instrus[id];
	obj->total_duration += (t2.tv_sec - obj->t1.tv_sec) * 1000000000ull + t2.tv_nsec - obj->t1.tv_nsec;
	obj->num_invocations++;
}

void instru_finalize(void);

#endif /* INSTRU_H */
