MOD_DIR := $(USERMOD_DIR)
SRC_USERMOD = $(MOD_DIR)/opusmod.c

# Opus Core Sources (subset, assuming others are pulled in by silk/celt if needed, or are float-only)
SRC_USERMOD += $(MOD_DIR)/libopus/src/opus.c
SRC_USERMOD += $(MOD_DIR)/libopus/src/opus_decoder.c
SRC_USERMOD += $(MOD_DIR)/libopus/src/opus_encoder.c
SRC_USERMOD += $(MOD_DIR)/libopus/src/opus_multistream.c
# SRC_USERMOD += $(MOD_DIR)/libopus/src/opus_multistream_decoder.c # Likely not needed for basic encode/decode
# SRC_USERMOD += $(MOD_DIR)/libopus/src/opus_multistream_encoder.c # Likely not needed for basic encode/decode
SRC_USERMOD += $(MOD_DIR)/libopus/src/repacketizer.c
SRC_USERMOD += $(MOD_DIR)/libopus/src/analysis.c # analysis.c might be float heavy, needs check. Or provide fixed point alternative.
# MLP is part of the new Neural PLC, might be complex for MCU. Consider removing if not essential.
SRC_USERMOD += $(MOD_DIR)/libopus/src/mlp.c
SRC_USERMOD += $(MOD_DIR)/libopus/src/mlp_data.c


# CELT sources - user needs to place these in libopus/src/celt/
# These should be the fixed-point versions if available, or generic C versions.
# The original Opus build system would select these.
SRC_USERMOD += $(wildcard $(MOD_DIR)/libopus/src/celt/*.c) # User must ensure these are appropriate for fixed-point build

# SILK sources - user needs to place these in libopus/src/silk/ and its subdirectories
# Crucially, only include fixed-point SILK sources.
SRC_USERMOD += $(wildcard $(MOD_DIR)/libopus/src/silk/*.c) # Generic SILK sources
SRC_USERMOD += $(wildcard $(MOD_DIR)/libopus/src/silk/fixed/*.c) # FIXED-POINT SILK sources
# DO NOT INCLUDE: $(wildcard $(MOD_DIR)/libopus/src/silk/float/*.c)

CFLAGS_USERMOD += -I$(MOD_DIR)/libopus/include # Public API
CFLAGS_USERMOD += -I$(MOD_DIR)/libopus/src     # For opus_private.h and internal includes
# Add include paths for CELT and SILK if their headers are within their respective directories
CFLAGS_USERMOD += -I$(MOD_DIR)/libopus/src/celt
CFLAGS_USERMOD += -I$(MOD_DIR)/libopus/src/silk
CFLAGS_USERMOD += -I$(MOD_DIR)/libopus/src/silk/fixed

# --- CRITICAL OPUS CONFIGURATION MACROS ---
CFLAGS_USERMOD += -DFIXED_POINT=1          # Enable fixed-point arithmetic
CFLAGS_USERMOD += -DDISABLE_FLOAT_API     # Disable float API (if applicable to the Opus version being used)
CFLAGS_USERMOD += -DOPUS_BUILD             # Building Opus library components
CFLAGS_USERMOD += -DUSE_ALLOCA=0           # Or simply do not define USE_ALLOCA
CFLAGS_USERMOD += -DEXPORT=""              # For static linking, ensure no DLL export macros interfere
CFLAGS_USERMOD += -Dinline=inline          # Ensure 'inline' keyword is treated as C99 inline
CFLAGS_USERMOD += -Drestrict=             # Already present, keep for compatibility

# Optional: Disable assembly optimizations if they cause issues or are not available
CFLAGS_USERMOD += -DDISABLE_OPTIMIZATIONS  # If Opus source uses this to disable complex/asm opts
CFLAGS_USERMOD += -DDISABLE_HR_TIMER       # If Opus has high-resolution timer code not for MCU
