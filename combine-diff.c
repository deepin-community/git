#include "git-compat-util.h"
#include "object-store-ll.h"
#include "convert.h"
#include "environment.h"
#include "hex.h"
#include "object-name.h"
#include "tree.h"
	if (!newline)
	if (!base) {
		blob = repo_read_object_file(r, oid, &type, size);
			 const char *func UNUSED, long funclen UNUSED)
		abb = repo_find_unique_abbrev(the_repository,
					      &elem->parent[i].oid, abbrev);
	abb = repo_find_unique_abbrev(the_repository, &elem->oid, abbrev);
	if (opt->ignore_regex_nr)
		die("combined diff and '%s' cannot be used together",
		    "--ignore-matching-lines");
	if (opt->close_file)
		die("combined diff and '%s' cannot be used together",
		    "--output");
