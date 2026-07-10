
# INIT SETTINGS

NAME = TESTPRGM
ICON = icon.png
DESCRIPTION = "this test prgm"

COMPRESSED = YES # for compressing big code
COMPRESSED_MODE = zx7 # zx0 for better results but longer time
ARCHIVED = YES
OUTPUT_MAP = NO # only for debug info

CFLAGS = -Wall -Wextra -Oz # C flags, these are nice standard ones
ASFLAGS = # asm flags, standard is none

LTO = YES # Decrease output size
PREFER_OS_CRT = NO # Can decrease size, prob not, might work
HAS_MATH_ERRNO = NO # Put Yes for better errno values
PREFER_OS_LIBC = YES # Decreases size by using calculator's own functions if exist
HAS_PRINTF = NO # for floats in printf()

# FILES
# Working dir (src) is always loaded
# "make gfx" will normally run convimg

include $(shell cedev-config --makefile)


