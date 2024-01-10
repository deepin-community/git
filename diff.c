#include "git-compat-util.h"
#include "abspath.h"
#include "base85.h"
#include "convert.h"
#include "environment.h"
#include "gettext.h"
#include "hex.h"
#include "object-store-ll.h"
#include "mem-pool.h"
#include "merge-ll.h"
#include "oid-array.h"
#include "pager.h"
#include "dir.h"
#include "object-file.h"
#include "object-name.h"
#include "read-cache-ll.h"
#include "setup.h"
#include "strmap.h"
#include "ws.h"
static int diff_stat_name_width;
		string_list_split_in_place(&params, params_copy, ",", -1);
int git_diff_heuristic_config(const char *var, const char *value,
			      void *cb UNUSED)
int git_diff_ui_config(const char *var, const char *value,
		       const struct config_context *ctx, void *cb)
		diff_context_default = git_config_int(var, value, ctx->kvi);
		diff_interhunk_context_default = git_config_int(var, value,
								ctx->kvi);
	if (!strcmp(var, "diff.statnamewidth")) {
		diff_stat_name_width = git_config_int(var, value, ctx->kvi);
		return 0;
	}
		diff_stat_graph_width = git_config_int(var, value, ctx->kvi);
	return git_diff_basic_config(var, value, ctx, cb);
int git_diff_basic_config(const char *var, const char *value,
			  const struct config_context *ctx, void *cb)
		diff_rename_limit_default = git_config_int(var, value, ctx->kvi);
	return git_default_config(var, value, ctx, cb);
static int count_trailing_blank(mmfile_t *mf)
		if (!ws_blank_line(prev_eol + 1, ptr - prev_eol))
	l1 = count_trailing_blank(mf1);
	l2 = count_trailing_blank(mf2);
	unsigned id;
#define EMITTED_DIFF_SYMBOL_INIT { 0 }
#define EMITTED_DIFF_SYMBOLS_INIT { 0 }
static void free_emitted_diff_symbols(struct emitted_diff_symbols *e)
{
	if (!e)
		return;
	free(e->buf);
	free(e);
}

	struct moved_entry *next_match;
			    const struct emitted_diff_symbol *b)
{
	int a_width = a->indent_width,
	if (a_width == INDENT_BLANKLINE && b_width == INDENT_BLANKLINE)
		return INDENT_BLANKLINE;
	return a_width - b_width;
static int cmp_in_block_with_wsd(const struct moved_entry *cur,
				 const struct emitted_diff_symbol *l,
				 struct moved_block *pmb)
{
	int a_width = cur->es->indent_width, b_width = l->indent_width;
	/* The text of each line must match */
	if (cur->es->id != l->id)
	/*
	 * If 'l' and 'cur' are both blank then we don't need to check the
	 * indent. We only need to check cur as we know the strings match.
	 * */
	if (a_width == INDENT_BLANKLINE)
	 * match those of the current block.
	delta = b_width - a_width;
	return delta != pmb->wsd;
struct interned_diff_symbol {
	struct hashmap_entry ent;
	struct emitted_diff_symbol *es;
};

static int interned_diff_symbol_cmp(const void *hashmap_cmp_fn_data,
				    const struct hashmap_entry *eptr,
				    const struct hashmap_entry *entry_or_key,
				    const void *keydata UNUSED)
	const struct emitted_diff_symbol *a, *b;
	a = container_of(eptr, const struct interned_diff_symbol, ent)->es;
	b = container_of(entry_or_key, const struct interned_diff_symbol, ent)->es;
	return !xdiff_compare_lines(a->line + a->indent_off,
				    a->len - a->indent_off,
				    b->line + b->indent_off,
				    b->len - b->indent_off, flags);
static void prepare_entry(struct diff_options *o, struct emitted_diff_symbol *l,
			  struct interned_diff_symbol *s)
	unsigned int hash = xdiff_hash_string(l->line + l->indent_off,
					      l->len - l->indent_off, flags);
	hashmap_entry_init(&s->ent, hash);
	s->es = l;
struct moved_entry_list {
	struct moved_entry *add, *del;
};

static struct moved_entry_list *add_lines_to_move_detection(struct diff_options *o,
							    struct mem_pool *entry_mem_pool)
	struct mem_pool interned_pool;
	struct hashmap interned_map;
	struct moved_entry_list *entry_list = NULL;
	size_t entry_list_alloc = 0;
	unsigned id = 0;

	hashmap_init(&interned_map, interned_diff_symbol_cmp, o, 8096);
	mem_pool_init(&interned_pool, 1024 * 1024);

		struct interned_diff_symbol key;
		struct emitted_diff_symbol *l = &o->emitted_symbols->buf[n];
		struct interned_diff_symbol *s;
		struct moved_entry *entry;
		if (l->s != DIFF_SYMBOL_PLUS && l->s != DIFF_SYMBOL_MINUS) {
			fill_es_indent_data(l);
		prepare_entry(o, l, &key);
		s = hashmap_get_entry(&interned_map, &key, ent, &key.ent);
		if (s) {
			l->id = s->es->id;
		} else {
			l->id = id;
			ALLOC_GROW_BY(entry_list, id, 1, entry_list_alloc);
			hashmap_add(&interned_map,
				    memcpy(mem_pool_alloc(&interned_pool,
							  sizeof(key)),
					   &key, sizeof(key)));
		}
		entry = mem_pool_alloc(entry_mem_pool, sizeof(*entry));
		entry->es = l;
		entry->next_line = NULL;
		if (prev_line && prev_line->es->s == l->s)
			prev_line->next_line = entry;
		prev_line = entry;
		if (l->s == DIFF_SYMBOL_PLUS) {
			entry->next_match = entry_list[l->id].add;
			entry_list[l->id].add = entry;
		} else {
			entry->next_match = entry_list[l->id].del;
			entry_list[l->id].del = entry;
		}

	hashmap_clear(&interned_map);
	mem_pool_discard(&interned_pool, 0);

	return entry_list;
				struct emitted_diff_symbol *l,
				int *pmb_nr)
	int i, j;

	for (i = 0, j = 0; i < *pmb_nr; i++) {
		int match;
		if (o->color_moved_ws_handling &
		    COLOR_MOVED_WS_ALLOW_INDENTATION_CHANGE)
			match = cur &&
				!cmp_in_block_with_wsd(cur, l, &pmb[i]);
		else
			match = cur && cur->es->id == l->id;
		if (match) {
			pmb[j] = pmb[i];
			pmb[j++].match = cur;
	*pmb_nr = j;
static void fill_potential_moved_blocks(struct diff_options *o,
					struct moved_entry *match,
					struct emitted_diff_symbol *l,
					struct moved_block **pmb_p,
					int *pmb_alloc_p, int *pmb_nr_p)
{
	struct moved_block *pmb = *pmb_p;
	int pmb_alloc = *pmb_alloc_p, pmb_nr = *pmb_nr_p;
	/*
	 * The current line is the start of a new block.
	 * Setup the set of potential blocks.
	 */
	for (; match; match = match->next_match) {
		ALLOC_GROW(pmb, pmb_nr + 1, pmb_alloc);
		if (o->color_moved_ws_handling &
		    COLOR_MOVED_WS_ALLOW_INDENTATION_CHANGE)
			pmb[pmb_nr].wsd = compute_ws_delta(l, match->es);
		else
			pmb[pmb_nr].wsd = 0;
		pmb[pmb_nr++].match = match;
	*pmb_p = pmb;
	*pmb_alloc_p = pmb_alloc;
	*pmb_nr_p = pmb_nr;
#define DIFF_SYMBOL_MOVED_LINE_ZEBRA_MASK \
  (DIFF_SYMBOL_MOVED_LINE | DIFF_SYMBOL_MOVED_LINE_ALT)
		o->emitted_symbols->buf[n - i].flags &= ~DIFF_SYMBOL_MOVED_LINE_ZEBRA_MASK;
				struct moved_entry_list *entry_list)
	enum diff_symbol moved_symbol = DIFF_SYMBOL_BINARY_DIFF_HEADER;
			match = entry_list[l->id].del;
			match = entry_list[l->id].add;
		if (pmb_nr && (!match || l->s != moved_symbol)) {
			if (!adjust_last_block(o, n, block_length) &&
			    block_length > 1) {
				/*
				 * Rewind in case there is another match
				 * starting at the second line of the block
				 */
				match = NULL;
				n -= block_length;
			}
		}
		if (!match) {
			moved_symbol = DIFF_SYMBOL_BINARY_DIFF_HEADER;
		pmb_advance_or_null(o, l, pmb, &pmb_nr);
			int contiguous = adjust_last_block(o, n, block_length);

			if (!contiguous && block_length > 1)
				/*
				 * Rewind in case there is another match
				 * starting at the second line of the block
				 */
				n -= block_length;
			else
				fill_potential_moved_blocks(o, match, l,
							    &pmb, &pmb_alloc,
							    &pmb_nr);
			if (contiguous && pmb_nr && moved_symbol == l->s)
			if (pmb_nr)
				moved_symbol = l->s;
			else
				moved_symbol = DIFF_SYMBOL_BINARY_DIFF_HEADER;

	struct emitted_diff_symbol e = {
		.line = line, .len = len, .flags = flags, .s = s
	};
	return ws_blank_line(line, len);
				  const char *func UNUSED, long funclen UNUSED)
		free_emitted_diff_symbols(ecbdata->diff_words->opt->emitted_symbols);
	if (!len)
		BUG("xdiff fed us an empty line");

		len = utf8_strwidth(file->print_name);
	/*
	 * NEEDSWORK: line_prefix is often used for "log --graph" output
	 * and contains ANSI-colored string.  utf8_strnwidth() should be
	 * used to correctly count the display width instead of strlen().
	 */
	if (options->stat_name_width == -1)
		options->stat_name_width = diff_stat_name_width;
	 * Guarantee 3/8*16 == 6 for the graph part
	 * and 5/8*16 == 10 for the filename part
		int name_len, padding;
		name_len = utf8_strwidth(name);
			/*
			 * NEEDSWORK: (name_len - len) counts the display
			 * width, which would be shorter than the byte
			 * length of the corresponding substring.
			 * Advancing "name" by that number of bytes does
			 * *NOT* skip over that many columns, so it is
			 * very likely that chomping the pathname at the
			 * slash we will find starting from "name" will
			 * leave the resulting string still too long.
			 */
		padding = len - utf8_strwidth(name);
		if (padding < 0)
			padding = 0;
			strbuf_addf(&out, " %s%s%*s | %*s",
				    prefix, name, padding, "",
				    number_width, "Bin");
			strbuf_addf(&out, " %s%s%*s | %*s",
				    prefix, name, padding, "",
				    number_width, "Unmerged\n");
		strbuf_addf(&out, " %s%s%*s | %*"PRIuMAX"%s",
			    prefix, name, padding, "",
			    number_width, added + deleted,
			    added + deleted ? " " : "");
static void conclude_dirstat(struct diff_options *options,
			     struct dirstat_dir *dir,
			     unsigned long changed)
{
	struct dirstat_file *to_free = dir->files;

	if (!changed) {
		/* This can happen even with many files, if everything was renames */
		;
	} else {
		/* Show all directories with more than x% of the changes */
		QSORT(dir->files, dir->nr, dirstat_compare);
		gather_dirstat(options, dir, changed, "", 0);
	}

	free(to_free);
}

	conclude_dirstat(options, &dir, changed);
	conclude_dirstat(options, &dir, changed);
				   long ob UNUSED, long on UNUSED,
				   long nb, long nn UNUSED,
				   const char *func UNUSED, long funclen UNUSED)
void diff_set_noprefix(struct diff_options *options)
{
	options->a_prefix = options->b_prefix = "";
}

void diff_set_default_prefix(struct diff_options *options)
{
	options->a_prefix = "a/";
	options->b_prefix = "b/";
}

static struct string_list *additional_headers(struct diff_options *o,
					      const char *path)
{
	if (!o->additional_path_headers)
		return NULL;
	return strmap_get(o->additional_path_headers, path);
}

static void add_formatted_header(struct strbuf *msg,
				  const char *header,
				  const char *line_prefix,
				  const char *meta,
				  const char *reset)
{
	const char *next, *newline;

	for (next = header; *next; next = newline) {
		newline = strchrnul(next, '\n');
		strbuf_addf(msg, "%s%s%.*s%s\n", line_prefix, meta,
			    (int)(newline - next), next, reset);
		if (*newline)
			newline++;
	}
}

static void add_formatted_headers(struct strbuf *msg,
				  struct string_list *more_headers,
				  const char *line_prefix,
				  const char *meta,
				  const char *reset)
{
	int i;

	for (i = 0; i < more_headers->nr; i++)
		add_formatted_header(msg, more_headers->items[i].string,
				     line_prefix, meta, reset);
}

static int diff_filepair_is_phoney(struct diff_filespec *one,
				   struct diff_filespec *two)
{
	/*
	 * This function specifically looks for pairs injected by
	 * create_filepairs_for_header_only_notifications().  Such
	 * pairs are "phoney" in that they do not represent any
	 * content or even mode difference, but were inserted because
	 * diff_queued_diff previously had no pair associated with
	 * that path but we needed some pair to avoid losing the
	 * "remerge CONFLICT" header associated with the path.
	 */
	return !DIFF_FILE_VALID(one) && !DIFF_FILE_VALID(two);
}

static int set_diff_algorithm(struct diff_options *opts,
			      const char *alg)
{
	long value = parse_algorithm_value(alg);

	if (value < 0)
		return -1;

	/* clear out previous settings */
	DIFF_XDL_CLR(opts, NEED_MINIMAL);
	opts->xdl_opts &= ~XDF_DIFF_ALGORITHM_MASK;
	opts->xdl_opts |= value;

	return 0;
}

	    (!two->mode || S_ISGITLINK(two->mode)) &&
	    (!diff_filepair_is_phoney(one, two))) {
		   (!two->mode || S_ISGITLINK(two->mode)) &&
		   (!diff_filepair_is_phoney(one, two))) {
	if (diff_filepair_is_phoney(one, two)) {
		/*
		 * We should only reach this point for pairs generated from
		 * create_filepairs_for_header_only_notifications().  For
		 * these, we want to avoid the "/dev/null" special casing
		 * above, because we do not want such pairs shown as either
		 * "new file" or "deleted file" below.
		 */
		lbl[0] = a_one;
		lbl[1] = b_two;
	}
		o->found_changes = 1;
		o->found_changes = 1;
			o->found_changes = 1;
	/*
	 * If this path does not match our sparse-checkout definition,
	 * then the file will not be in the working directory.
	 */
	if (!path_in_sparse_checkout(name, istate))
		return 0;

	temp->tempfile = mks_tempfile_dt("git-blob-XXXXXX", base);
	     reuse_worktree_file(r->index, one->path, &one->oid, 1))) {
		if (lstat(one->path, &st) < 0) {
			die_errno("stat(%s)", one->path);
			if (strbuf_readlink(&sb, one->path, st.st_size) < 0)
				die_errno("readlink(%s)", one->path);
			prep_temp_blob(r->index, one->path, temp, sb.buf, sb.len,
			temp->name = one->path;
		prep_temp_blob(r->index, one->path, temp,
	struct diff_tempfile *temp = prepare_temp_file(r, df);
	struct child_process cmd = CHILD_PROCESS_INIT;
	strvec_push(&cmd.args, pgm);
	strvec_push(&cmd.args, name);
		add_external_diff_name(o->repo, &cmd.args, one);
		add_external_diff_name(o->repo, &cmd.args, two);
		if (other) {
			strvec_push(&cmd.args, other);
			strvec_push(&cmd.args, xfrm_msg);
	strvec_pushf(&cmd.env, "GIT_DIFF_PATH_COUNTER=%d",
		     ++o->diff_path_counter);
	strvec_pushf(&cmd.env, "GIT_DIFF_PATH_TOTAL=%d", q->nr);
	cmd.use_shell = 1;
	if (run_command(&cmd))
		return repo_find_unique_abbrev(the_repository, oid, abbrev);
	struct string_list *more_headers = NULL;
	if ((more_headers = additional_headers(o, name))) {
		add_formatted_headers(msg, more_headers,
				      line_prefix, set, reset);
		*must_show_header = 1;
	}
	struct userdiff_driver *drv = NULL;
	if (o->flags.allow_external || !o->ignore_driver_algorithm)

	if (o->flags.allow_external && drv && drv->external)
		pgm = drv->external;
	if (one && two) {
		if (!o->ignore_driver_algorithm && drv && drv->algorithm)
			set_diff_algorithm(o, drv->algorithm);

	} else {
	}
	if (!o->ignore_driver_algorithm) {
		struct userdiff_driver *drv = userdiff_find_by_path(o->repo->index,
								    p->one->path);

		if (drv && drv->algorithm)
			set_diff_algorithm(o, drv->algorithm);
	}

		diff_set_noprefix(options);
		diff_set_default_prefix(options);
}

static const char diff_status_letters[] = {
	DIFF_STATUS_ADDED,
	DIFF_STATUS_COPIED,
	DIFF_STATUS_DELETED,
	DIFF_STATUS_MODIFIED,
	DIFF_STATUS_RENAMED,
	DIFF_STATUS_TYPE_CHANGED,
	DIFF_STATUS_UNKNOWN,
	DIFF_STATUS_UNMERGED,
	DIFF_STATUS_FILTER_AON,
	DIFF_STATUS_FILTER_BROKEN,
	'\0',
};

static unsigned int filter_bit['Z' + 1];

static void prepare_filter_bits(void)
{
	int i;

	if (!filter_bit[DIFF_STATUS_ADDED]) {
		for (i = 0; diff_status_letters[i]; i++)
			filter_bit[(int) diff_status_letters[i]] = (1 << i);
	}
}

static unsigned filter_bit_tst(char status, const struct diff_options *opt)
{
	return opt->filter & filter_bit[(int) status];
}

unsigned diff_filter_bit(char status)
{
	prepare_filter_bits();
	return filter_bit[(int) status];
}
int diff_check_follow_pathspec(struct pathspec *ps, int die_on_error)
{
	unsigned forbidden_magic;

	if (ps->nr != 1) {
		if (die_on_error)
			die(_("--follow requires exactly one pathspec"));
		return 0;
	}

	forbidden_magic = ps->items[0].magic & ~(PATHSPEC_FROMTOP |
						 PATHSPEC_LITERAL);
	if (forbidden_magic) {
		if (die_on_error) {
			struct strbuf sb = STRBUF_INIT;
			pathspec_magic_names(forbidden_magic, &sb);
			die(_("pathspec magic not supported by --follow: %s"),
			    sb.buf);
		}
		return 0;
	}

	return 1;
		die(_("options '%s', '%s', '%s', and '%s' cannot be used together"),
			"--name-only", "--name-status", "--check", "-s");
		die(_("options '%s', '%s', and '%s' cannot be used together"),
			"-G", "-S", "--find-object");
		die(_("options '%s' and '%s' cannot be used together, use '%s' with '%s'"),
			"-G", "--pickaxe-regex", "--pickaxe-regex", "-S");
		die(_("options '%s' and '%s' cannot be used together, use '%s' with '%s' and '%s'"),
			"--pickaxe-all", "--find-object", "--pickaxe-all", "-G", "-S");
	/*
	 * --name-only, --name-status, --checkdiff, and -s
	 * turn other output format off.
	 */
	if (options->flags.follow_renames)
		diff_check_follow_pathspec(&options->pathspec, 1);
	if (options->filter_not) {
		if (!options->filter)
			options->filter = ~filter_bit[DIFF_STATUS_FILTER_AON];
		options->filter &= ~options->filter_not;
	}
	options->output_format &= ~DIFF_FORMAT_NO_OUTPUT;
	options->output_format &= ~DIFF_FORMAT_NO_OUTPUT;
			opt->filter_not |= bit;
	if (repo_get_oid(the_repository, arg, &oid))
		options->output_format &= ~DIFF_FORMAT_NO_OUTPUT;

	if (set_diff_algorithm(options, arg))
	options->ignore_driver_algorithm = 1;

	return 0;
}

static int diff_opt_diff_algorithm_no_arg(const struct option *opt,
				   const char *arg, int unset)
{
	struct diff_options *options = opt->value;

	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);

	if (set_diff_algorithm(options, opt->long_name))
		BUG("available diff algorithms include \"myers\", "
			       "\"minimal\", \"patience\" and \"histogram\"");

	options->ignore_driver_algorithm = 1;

	diff_set_noprefix(options);
	return 0;
}

static int diff_opt_default_prefix(const struct option *opt,
				   const char *optarg, int unset)
{
	struct diff_options *options = opt->value;

	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(optarg);
	diff_set_default_prefix(options);
	options->ignore_driver_algorithm = 1;

	return set_diff_algorithm(options, "patience");
/*
 * Consider adding new flags to __git_diff_common_options
 * in contrib/completion/git-completion.bash
 */
struct option *add_diff_options(const struct option *opts,
				struct diff_options *options)
		OPT_SET_INT('s', "no-patch", &options->output_format,
			    N_("suppress diff output"), DIFF_FORMAT_NO_OUTPUT),
		OPT_BITOP(0, "raw", &options->output_format,
			  DIFF_FORMAT_RAW, DIFF_FORMAT_NO_OUTPUT),
		OPT_BITOP(0, "numstat", &options->output_format,
			  DIFF_FORMAT_NUMSTAT, DIFF_FORMAT_NO_OUTPUT),
		OPT_BITOP(0, "shortstat", &options->output_format,
			  DIFF_FORMAT_SHORTSTAT, DIFF_FORMAT_NO_OUTPUT),
		OPT_BITOP(0, "summary", &options->output_format,
			  DIFF_FORMAT_SUMMARY, DIFF_FORMAT_NO_OUTPUT),
		OPT_CALLBACK_F(0, "default-prefix", options, NULL,
			       N_("use default prefixes a/ and b/"),
			       PARSE_OPT_NONEG | PARSE_OPT_NOARG, diff_opt_default_prefix),
		OPT_CALLBACK_F(0, "minimal", options, NULL,
			       N_("produce the smallest possible diff"),
			       PARSE_OPT_NONEG | PARSE_OPT_NOARG,
			       diff_opt_diff_algorithm_no_arg),
		OPT_CALLBACK_F(0, "histogram", options, NULL,
			       N_("generate diff using the \"histogram diff\" algorithm"),
			       PARSE_OPT_NONEG | PARSE_OPT_NOARG,
			       diff_opt_diff_algorithm_no_arg),
		  N_("output to a specific file"),
	return parse_options_concat(opts, parseopts);
	struct option no_options[] = { OPT_END() };
	struct option *parseopts = add_diff_options(no_options, options);

	ac = parse_options(ac, av, prefix, parseopts, NULL,
			   PARSE_OPT_KEEP_UNKNOWN_OPT |
	free(parseopts);
void diff_free_queue(struct diff_queue_struct *q)
{
	for (int i = 0; i < q->nr; i++)
		diff_free_filepair(q->queue[i]);
	free(q->queue);
}

	int include_conflict_headers =
	    (additional_headers(o, p->one->path) &&
	     !o->pickaxe_opts &&
	     (!o->filter || filter_bit_tst(DIFF_STATUS_UNMERGED, o)));

	/*
	 * Check if we can return early without showing a diff.  Note that
	 * diff_filepair only stores {oid, path, mode, is_valid}
	 * information for each path, and thus diff_unmodified_pair() only
	 * considers those bits of info.  However, we do not want pairs
	 * created by create_filepairs_for_header_only_notifications()
	 * (which always look like unmodified pairs) to be ignored, so
	 * return early if both p is unmodified AND we don't want to
	 * include_conflict_headers.
	 */
	if (diff_unmodified_pair(p) && !include_conflict_headers)
	/* Actually, we can also return early to avoid showing tree diffs */
		return;
int diff_queue_is_empty(struct diff_options *o)
	int include_conflict_headers =
	    (o->additional_path_headers &&
	     strmap_get_size(o->additional_path_headers) &&
	     !o->pickaxe_opts &&
	     (!o->filter || filter_bit_tst(DIFF_STATUS_UNMERGED, o)));

	if (include_conflict_headers)
		return 0;


	opt->found_changes = 1;
static int diff_get_patch_id(struct diff_options *options, struct object_id *oid, int diff_header_only)
		} else if (p->one->mode != p->two->mode) {
			patch_id_add_string(&ctx, "oldmode");
			patch_id_add_mode(&ctx, p->one->mode);
			patch_id_add_string(&ctx, "newmode");
			patch_id_add_mode(&ctx, p->two->mode);
		if (diff_header_only) {
			/* don't do anything since we're only populating header info */
		} else if (diff_filespec_is_binary(options->repo, p->one) ||
		} else {
			if (p->one->mode == 0) {
				patch_id_add_string(&ctx, "---/dev/null");
				patch_id_add_string(&ctx, "+++b/");
				the_hash_algo->update_fn(&ctx, p->two->path, len2);
			} else if (p->two->mode == 0) {
				patch_id_add_string(&ctx, "---a/");
				the_hash_algo->update_fn(&ctx, p->one->path, len1);
				patch_id_add_string(&ctx, "+++/dev/null");
			} else {
				patch_id_add_string(&ctx, "---a/");
				the_hash_algo->update_fn(&ctx, p->one->path, len1);
				patch_id_add_string(&ctx, "+++b/");
				the_hash_algo->update_fn(&ctx, p->two->path, len2);
			}
			if (fill_mmfile(options->repo, &mf1, p->one) < 0 ||
			    fill_mmfile(options->repo, &mf2, p->two) < 0)
				return error("unable to read files to diff");
			xpp.flags = 0;
			xecfg.ctxlen = 3;
			xecfg.flags = XDL_EMIT_NO_HUNK_HDR;
			if (xdi_diff_outf(&mf1, &mf2, NULL,
					  patch_id_consume, &data, &xpp, &xecfg))
				return error("unable to generate patch-id diff for %s",
					     p->one->path);
		}
		flush_one_hunk(oid, &ctx);
int diff_flush_patch_id(struct diff_options *options, struct object_id *oid, int diff_header_only)
	int result = diff_get_patch_id(options, oid, diff_header_only);
	diff_free_queue(q);
static void create_filepairs_for_header_only_notifications(struct diff_options *o)
{
	struct strset present;
	struct diff_queue_struct *q = &diff_queued_diff;
	struct hashmap_iter iter;
	struct strmap_entry *e;
	int i;

	strset_init_with_options(&present, /*pool*/ NULL, /*strdup*/ 0);

	/*
	 * Find out which paths exist in diff_queued_diff, preferring
	 * one->path for any pair that has multiple paths.
	 */
	for (i = 0; i < q->nr; i++) {
		struct diff_filepair *p = q->queue[i];
		char *path = p->one->path ? p->one->path : p->two->path;

		if (strmap_contains(o->additional_path_headers, path))
			strset_add(&present, path);
	}

	/*
	 * Loop over paths in additional_path_headers; for each NOT already
	 * in diff_queued_diff, create a synthetic filepair and insert that
	 * into diff_queued_diff.
	 */
	strmap_for_each_entry(o->additional_path_headers, &iter, e) {
		if (!strset_contains(&present, e->key)) {
			struct diff_filespec *one, *two;
			struct diff_filepair *p;

			one = alloc_filespec(e->key);
			two = alloc_filespec(e->key);
			fill_filespec(one, null_oid(), 0, 0);
			fill_filespec(two, null_oid(), 0, 0);
			p = diff_queue(q, one, two);
			p->status = DIFF_STATUS_MODIFIED;
		}
	}

	/* Re-sort the filepairs */
	diffcore_fix_diff_index();

	/* Cleanup */
	strset_clear(&present);
}

	if (o->additional_path_headers)
		create_filepairs_for_header_only_notifications(o);

			struct mem_pool entry_pool;
			struct moved_entry_list *entry_list;
			mem_pool_init(&entry_pool, 1024 * 1024);
			entry_list = add_lines_to_move_detection(o,
								 &entry_pool);
			mark_color_as_moved(o, entry_list);
			mem_pool_discard(&entry_pool, 0);
			free(entry_list);
	clear_pathspec(&options->pathspec);
	if (!q->nr && !options->additional_path_headers)
	if (output_format & DIFF_FORMAT_PATCH) {
		if (separator) {
			emit_diff_symbol(options, DIFF_SYMBOL_SEPARATOR, NULL, 0, 0);
			if (options->stat_sep)
				/* attach patch instead of inline */
				emit_diff_symbol(options, DIFF_SYMBOL_STAT_SEP,
						 NULL, 0, 0);
		}

		diff_flush_patch_all_file_pairs(options);
	}

	if (output_format & DIFF_FORMAT_CALLBACK)
		options->format_callback(q, options, options->format_callback_data);

	diff_free_queue(q);
void init_diffstat_widths(struct diff_options *options)
{
	options->stat_width = -1;        /* use full terminal width */
	options->stat_name_width = -1;   /* respect diff.statNameWidth config */
	options->stat_graph_width = -1;  /* respect diff.statGraphWidth config */
}

	if (options->repo == the_repository && repo_has_promisor_remote(the_repository) &&
int diff_result_code(struct diff_options *opt)

	options->found_changes = !!diffstat->nr;
	temp = prepare_temp_file(r, spec);
	strvec_push(&child.args, pgm);
	strvec_push(&child.args, temp->name);