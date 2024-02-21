all:
	$(CC) -fPIE -static -Wl,--gc-sections -fstack-protector-all -fstack-clash-protection -mshstk  -D_FORTIFY_SOURCE=3 -Wno-unused-result -O2 -std=gnu99 -Wno-gnu-zero-variadic-macro-arguments -o cpulog cpu.c -z noexecstack -z now
	$(CC) -fPIE -static -Wl,--gc-sections -fstack-protector-all -fstack-clash-protection -mshstk  -D_FORTIFY_SOURCE=3 -Wno-unused-result -O2 -std=gnu99 -Wno-gnu-zero-variadic-macro-arguments -o cpubt bt.c -z noexecstack -z now
	strip cpulog
	strip cpubt
	cp cpulog module/
	cp cpubt module/system/bin/
	cd module&&zip -r ../cpulog.zip .
format:
	clang-format -i *.c
dev:
	$(CC) -std=gnu99 -fPIE -ggdb -O0 -fno-omit-frame-pointer -z norelro -z execstack -Wno-gnu-zero-variadic-macro-arguments -fno-stack-protector -Wall -Wextra -pedantic -Wconversion -Wno-newline-eof -o cpubt cpubt.c
	$(CC) -std=gnu99 -fPIE -ggdb -O0 -fno-omit-frame-pointer -z norelro -z execstack -Wno-gnu-zero-variadic-macro-arguments -fno-stack-protector -Wall -Wextra -pedantic -Wconversion -Wno-newline-eof -o cpulog cpu.c
check:
	clang-tidy --checks=*,-clang-analyzer-security.insecureAPI.strcpy,-altera-unroll-loops,-cert-err33-c,-concurrency-mt-unsafe,-clang-analyzer-security.insecureAPI.DeprecatedOrUnsafeBufferHandling,-readability-function-cognitive-complexity,-cppcoreguidelines-avoid-magic-numbers,-readability-magic-numbers,-bugprone-easily-swappable-parameters,-cert-err34-c,-misc-include-cleaner,-readability-identifier-length,-bugprone-signal-handler,-cert-msc54-cpp,-cert-sig30-c,-altera-id-dependent-backward-branch,-cppcoreguidelines-avoid-non-const-global-variables *.c --
