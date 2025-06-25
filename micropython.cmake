set(SRC_USERMOD
    ${CMAKE_CURRENT_LIST_DIR}/opusmod.c
    ${CMAKE_CURRENT_LIST_DIR}/libopus/src/opus.c
    ${CMAKE_CURRENT_LIST_DIR}/libopus/src/opus_decoder.c
    ${CMAKE_CURRENT_LIST_DIR}/libopus/src/opus_encoder.c
    ${CMAKE_CURRENT_LIST_DIR}/libopus/src/opus_multistream.c
    ${CMAKE_CURRENT_LIST_DIR}/libopus/src/opus_multistream_decoder.c
    ${CMAKE_CURRENT_LIST_DIR}/libopus/src/opus_multistream_encoder.c
    ${CMAKE_CURRENT_LIST_DIR}/libopus/src/repacketizer.c
    ${CMAKE_CURRENT_LIST_DIR}/libopus/src/analysis.c
    ${CMAKE_CURRENT_LIST_DIR}/libopus/src/mlp.c
    ${CMAKE_CURRENT_LIST_DIR}/libopus/src/mlp_data.c
    # 批量添加 silk 和 celt
    ${CMAKE_CURRENT_LIST_DIR}/libopus/src/silk/CNG.c
    ${CMAKE_CURRENT_LIST_DIR}/libopus/src/silk/code_signs.c
    # ... silk/*.c 所有文件
    ${CMAKE_CURRENT_LIST_DIR}/libopus/src/celt/bands.c
    # ... celt/*.c 所有文件
)
set(INCLUDE_USERMOD
    ${CMAKE_CURRENT_LIST_DIR}/libopus/include
)
set(CFLAGS_USERMOD
    -DOPUS_BUILD
    -Drestrict=
)
