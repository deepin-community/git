test_expect_success 'warn about add.interactive.useBuiltin' '
	cat >expect <<-\EOF &&
	warning: the add.interactive.useBuiltin setting has been removed!
	See its entry in '\''git help config'\'' for details.
	No changes.
	EOF

	for v in = =true =false
	do
		git -c "add.interactive.useBuiltin$v" add -p >out 2>actual &&
		test_must_be_empty out &&
		test_cmp expect actual || return 1
	done
'

test_expect_success 'update can stage deletions' '
	>to-delete &&
	git add to-delete &&
	rm to-delete &&
	test_write_lines u t "" | git add -i &&
	git ls-files to-delete >output &&
	test_must_be_empty output
'

	git diff --cached file >out &&
	grep "new mode" out &&
	grep "+content" out &&
	git diff file >out &&
	test_must_be_empty out
	test_grep "No changes" err &&
	test_grep "Only binary files changed" err
	echo content >>file &&
	test_write_lines A B C D>file2 &&
	git add file2
	diff --git a/file2 b/file2
	index 8422d40..35b930a 100644
	--- a/file2
	+++ b/file2
	@@ -1,4 +1,5 @@
	-A
	+Z
	 B
	+Y
	 C
	-D
	+X
	sed -e "/^index 180b47c/s/ 100644/ 100755/" \
	    -e /1,5/s//1,4/ \
	    -e /Y/d patch >>expected &&
	--- a/file2
	+++ b/file2
	@@ -1,4 +1,5 @@
	-A
	+Z
	 B
	+Y
	 C
	-D
	+X
	@@ -1,2 +1,2 @@
	-A
	+Z
	 B
	@@ -2,2 +2,3 @@
	 B
	+Y
	 C
	@@ -3,2 +4,2 @@
	 C
	-D
	+X
	test_write_lines s y y s y n y | git add -p 2>error >raw-output &&
	sed -n -e "s/^([1-9]\/[1-9]) Stage this hunk[^@]*\(@@ .*\)/\1/" \
	       -e "/^[-+@ \\\\]"/p raw-output >output &&