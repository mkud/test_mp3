# autotools does not allow you to specify "-Wl,-Bstatic" in "LDADD"
# this need for static linked "libmp3lame" but dynamic "pthread"
# But if I add "LDADD" as a comment to "Makefile.am", then it will also get added into the resulting "Makefile" as a comment
# So I simply uncomment this string after "configure"

# If I add "libmp3lame" to "LDFLAGS" as autotools wants, then ldd cannot link "libmp3lame". It should be after sources files in command line
make clean && aclocal && automake --foreign --add-missing && autoconf && ./configure && sed -i -e 's/#___testmp3_LDADD/___testmp3_LDADD/g' ./src/Makefile  &&  make
