# test_mp3

Task description you can find in Task.md

* src - source dir
* test_wav - set of test wav-files (Downloaded from https://freesound.org/ - free or NonCommercial License)
* `recompile_autotools_with_hack.sh` - compile project by autotools. Contains one hack (`./configure && make` - doesn't work).

I want to link `libmp3lame` statically, but `pthread` dynamically. If I add `libmp3lame` with `-Wl,-Bstatic` to `LDFLAGS`, then ldd cannot link `libmp3lame`. `libmp3lame` should to specify in `LDADD`, but autotools does not allow it. 
If I add `LDADD` as a comment to `src/Makefile.am`, then it will also get added into the resulting `Makefile` as a comment. So I simply uncomment this string after `configure` script and as a result, statically linked `libmp3lame` is appended to `LDADD`

* result file `testmp3`. To run the tests, run `testmp3 ./test_wav`