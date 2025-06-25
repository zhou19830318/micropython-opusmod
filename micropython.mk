MOD_DIR := $(USERMOD_DIR)
SRC_USERMOD += $(MOD_DIR)/opusmod.c
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
SRC_USERMOD += $(MOD_DIR)/libopus/src/silk/*.c
SRC_USERMOD += $(MOD_DIR)/libopus/src/celt/*.c

CFLAGS_USERMOD += -I$(MOD_DIR)/libopus/include
CFLAGS_USERMOD += -DOPUS_BUILD
CFLAGS_USERMOD += -Drestrict=
