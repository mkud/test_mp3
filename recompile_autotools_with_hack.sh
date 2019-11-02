make clean
aclocal
automake --foreign --add-missing
autoconf

./configure

# I want to link "libmp3lame" statically, but "pthread" dynamically
# If I add "libmp3lame" with "-Wl,-Bstatic" to "LDFLAGS", then ldd cannot link "libmp3lame".
# "libmp3lame" should to specify in "LDADD", but autotools does not allow it.

# If I add "LDADD" as a comment to "Makefile.am", then it will also get added into the resulting "Makefile" as a comment
# So I simply uncomment this string after "configure" script
# and as a result, statically linked "libmp3lame" is appended to "LDADD"

sed -i -e 's/#___testmp3_LDADD/___testmp3_LDADD/g' ./src/Makefile

make
