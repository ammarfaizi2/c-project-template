// SPDX-License-Identifier: GPL-2.0
/*
 *  src/bluetea/teatest/teatest.c
 *
 *  Tea Test (Unit Test Framework for C project) header.
 *
 *  Copyright (C) 2021  Ammar Faizi
 */


#ifndef SRC____BLUETEA__TEATEST__TEATEST_H
#define SRC____BLUETEA__TEATEST__TEATEST_H


#include <libgen.h>
#include <stdbool.h>
#include <stdint.h>


typedef struct _point_t {
	uint32_t	total_point;
	uint32_t	point;
} point_t;


bool tq_assert_is_exec(void);
bool print_test(bool is_success, const char *func, const char *file, int line);


#define FN_TEATEST(PACKAGE, NAME) test_##PACKAGE##_##NAME
#define TEATEST(PACKAGE, NAME)					\
int FN_TEATEST(PACKAGE, NAME)(uint32_t *____total_point,	\
			      uint32_t *____point)		\


typedef int (*test_entry_t)(uint32_t *____total_point, uint32_t *____point);


#define TQ_START int ____ret = (0)
#define TQ_RETURN return ____ret
#define TQ_IF_RUN if (tq_assert_hook())


#define TQ_ASSERT(EXPR)							\
do {									\
	bool __is_success;						\
	char __fn0[1024] = __FILE__;					\
	char __fn1[1024] = __FILE__;					\
	char __file[2048];						\
									\
	snprintf(__file, sizeof(__file), "%s/%s",			\
		 basename(dirname(__fn0)), basename(__fn1));		\
									\
	if (tq_assert_is_exec()) {					\
		__is_success = (EXPR);					\
		if (print_test(__is_success, __func__ + 5, __file,	\
	                       __LINE__))				\
			(*____point)++;					\
		else							\
			____ret = 1;					\
	} else {							\
		(*____total_point)++;					\
	}								\
} while (0)


#define TQ_VOID(EXPR)			\
do {					\
	if (tq_assert_is_exec()) {	\
		(EXPR);			\
	}				\
} while (0)



#endif /* #ifndef SRC____BLUETEA__TEATEST__TEATEST_H */