// SPDX-License-Identifier: GPL-2.0
/*
 *  src/bluetea/lib/getopt.c
 *
 *  Getopt library
 *
 *  Copyright (C) 2021  Ammar Faizi
 */

#include <string.h>
#include <bluetea/lib/getopt.h>


static inline bool is_end_of_getopt_long(const struct bt_getopt_long *in)
{
	const struct bt_getopt_long cmp = GETOPT_LONG_STRUCT_END;

	/*
	 * Don't use memcmp!
	 *
	 * According to C standards (C99, C11 and C18):
	 * The value of padding bytes when storing values in
	 * structures or unions is unspecified (6.2.6.1).
	 *
	 */
	return (
		(cmp.opt == in->opt) &&
		(cmp.arg_req == in->arg_req) &&
		(cmp.val == in->val)
	);
}


static int track_getopt_long(const char *cur_arg, struct bt_getopt_wr *wr)
{
	int argc = wr->argc;
	int cur_idx = wr->cur_idx;
	const struct bt_getopt_long *long_opt = wr->long_opt;

	if (long_opt == NULL)
		goto out_unknown;

	while (!is_end_of_getopt_long(long_opt)) {


		if (strcmp(cur_arg, long_opt->opt)) {
			long_opt++;
			continue;
		}


		switch (long_opt->arg_req) {
		case NO_ARG:
			wr->retval = NULL;
			break;
		case REQUIRED_ARG: {
			const char *next_arg;

			if (cur_idx >= argc)
				return BT_GETOPT_MISSING_ARG;

			next_arg = wr->argv[cur_idx];
			if ((next_arg == NULL) || (*next_arg == '-'))
				return BT_GETOPT_MISSING_ARG;

			wr->cur_idx++;
			wr->retval = next_arg;
			break;
		}
		case OPTIONAL_ARG:
			break;
		default:
			return -BT_GETOPT_EINVAL;
		}


		return long_opt->val;
	}

out_unknown:
	wr->retval = NULL;
	return BT_GETOPT_UNKNOWN;
}


static int track_getopt_short(unsigned char c, struct bt_getopt_wr *wr)
{

}


int bt_getopt(struct bt_getopt_wr *wr)
{
	int ret;
	int argc = wr->argc;
	int cur_idx = wr->cur_idx++;
	const char **argv = wr->argv;
	const char *cur_arg;
	size_t cur_len;

	if (cur_idx >= argc)
		return BT_GETOPT_ENDED;


	cur_arg = argv[cur_idx];
	if (cur_arg[0] == '\0' || cur_arg[1] == '\0' || cur_arg[2] == '\0')
		goto out_no_opt;

	if (cur_arg[0] == '-') {
		if (cur_arg[1] == '-')
			return track_getopt_long(cur_arg + 2, wr);
		else
			return track_getopt_short(cur_arg[1], wr);
	}

out_no_opt:
	wr->retval = cur_arg;
	return BT_GETOPT_NO_OPT;
}
