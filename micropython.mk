MOD_DIR := $(USERMOD_DIR)
SRC_USERMOD = $(MOD_DIR)/opusmod.c
SRC_USERMOD += $(MOD_DIR)/libopus/src/opus.c
SRC_USERMOD += $(MOD_DIR)/libopus/src/opus_decoder.c
SRC_USERMOD += $(MOD_DIR)/libopus/src/opus_encoder.c
SRC_USERMOD += $(MOD_DIR)/libopus/src/opus_multistream.c
SRC_USERMOD += $(MOD_DIR)/libopus/src/opus_multistream_decoder.c
SRC_USERMOD += $(MOD_DIR)/libopus/src/opus_multistream_encoder.c
SRC_USERMOD += $(MOD_DIR)/libopus/src/repacketizer.c
SRC_USERMOD += $(MOD_DIR)/libopus/src/analysis.c
SRC_USERMOD += $(MOD_DIR)/libopus/src/mlp.c
SRC_USERMOD += $(MOD_DIR)/libopus/src/mlp_data.c

# CELT sources - user needs to place these in libopus/src/celt/
SRC_USERMOD += $(wildcard $(MOD_DIR)/libopus/src/celt/*.c)

# SILK sources - user needs to place these in libopus/src/silk/ and its subdirectories
SRC_USERMOD += $(wildcard $(MOD_DIR)/libopus/src/silk/*.c)
SRC_USERMOD += $(wildcard $(MOD_DIR)/libopus/src/silk/fixed/*.c)
SRC_USERMOD += $(wildcard $(MOD_DIR)/libopus/src/silk/float/*.c)

CFLAGS_USERMOD += -I$(MOD_DIR)/libopus/include # Public API
CFLAGS_USERMOD += -I$(MOD_DIR)/libopus/src     # For opus_private.h and internal includes
CFLAGS_USERMOD += -DOPUS_BUILD
CFLAGS_USERMOD += -Drestrict=
