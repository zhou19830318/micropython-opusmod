#include "py/obj.h"
#include "py/runtime.h"
#include "py/binary.h"
#include "opus.h"

// ----------- OpusDecoder 对象 -----------

typedef struct _opusmod_decoder_obj_t {
    mp_obj_base_t base;
    OpusDecoder *decoder;
    int sample_rate;
    int channels;
} opusmod_decoder_obj_t;

STATIC void opusmod_decoder_del(mp_obj_t self_in) {
    opusmod_decoder_obj_t *self = MP_OBJ_TO_PTR(self_in);
    if (self->decoder) {
        opus_decoder_destroy(self->decoder);
        self->decoder = NULL;
    }
}

STATIC mp_obj_t opusmod_decoder_make_new(const mp_obj_type_t *type,
                                         size_t n_args, size_t n_kw, const mp_obj_t *args) {
    mp_arg_check_num(n_args, n_kw, 2, 2, false);
    int sample_rate = mp_obj_get_int(args[0]);
    int channels = mp_obj_get_int(args[1]);
    int err;
    opusmod_decoder_obj_t *self = m_new_obj(opusmod_decoder_obj_t);
    self->base.type = type;
    self->sample_rate = sample_rate;
    self->channels = channels;
    self->decoder = opus_decoder_create(sample_rate, channels, &err);
    if (!self->decoder || err != OPUS_OK) {
        mp_raise_msg_varg(&mp_type_RuntimeError, "Opus decoder init failed: %d", err);
    }
    return MP_OBJ_FROM_PTR(self);
}

STATIC mp_obj_t opusmod_decoder_decode(mp_obj_t self_in, mp_obj_t data_in) {
    opusmod_decoder_obj_t *self = MP_OBJ_TO_PTR(self_in);
    mp_buffer_info_t bufinfo;
    mp_get_buffer_raise(data_in, &bufinfo, MP_BUFFER_READ);
    int max_frame_size = 5760; // 120ms @ 48kHz
    int16_t *pcm = m_new(int16_t, max_frame_size * self->channels);
    int frame_size = opus_decode(self->decoder, bufinfo.buf, bufinfo.len, pcm, max_frame_size, 0);
    if (frame_size < 0) {
        m_del(int16_t, pcm, max_frame_size * self->channels);
        mp_raise_msg_varg(&mp_type_RuntimeError, "Opus decode failed: %d", frame_size);
    }
    size_t out_bytes = frame_size * self->channels * sizeof(int16_t);
    mp_obj_t tuple[2];
    tuple[0] = mp_obj_new_bytes((const byte *)pcm, out_bytes);
    tuple[1] = mp_obj_new_int(frame_size);
    m_del(int16_t, pcm, max_frame_size * self->channels);
    return mp_obj_new_tuple(2, tuple);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_2(opusmod_decoder_decode_obj, opusmod_decoder_decode);

STATIC const mp_rom_map_elem_t opusmod_decoder_locals_dict_table[] = {
    { MP_ROM_QSTR(MP_QSTR_decode), MP_ROM_PTR(&opusmod_decoder_decode_obj) },
};
STATIC MP_DEFINE_CONST_DICT(opusmod_decoder_locals_dict, opusmod_decoder_locals_dict_table);

STATIC const mp_obj_type_t opusmod_decoder_type = {
    .base = { &mp_type_type },
    .name = MP_QSTR_OpusDecoder,
    .make_new = opusmod_decoder_make_new,
    .locals_dict = (mp_obj_dict_t*)&opusmod_decoder_locals_dict,
    .del = opusmod_decoder_del,
};

// ----------- OpusEncoder 对象 -----------

typedef struct _opusmod_encoder_obj_t {
    mp_obj_base_t base;
    OpusEncoder *encoder;
    int sample_rate;
    int channels;
    int application;
    int bitrate;
} opusmod_encoder_obj_t;

STATIC void opusmod_encoder_del(mp_obj_t self_in) {
    opusmod_encoder_obj_t *self = MP_OBJ_TO_PTR(self_in);
    if (self->encoder) {
        opus_encoder_destroy(self->encoder);
        self->encoder = NULL;
    }
}

STATIC mp_obj_t opusmod_encoder_make_new(const mp_obj_type_t *type,
                                         size_t n_args, size_t n_kw, const mp_obj_t *args) {
    enum { ARG_sample_rate, ARG_channels, ARG_application, ARG_bitrate };
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_sample_rate, MP_ARG_REQUIRED | MP_ARG_INT, {.u_int = 48000} },
        { MP_QSTR_channels,    MP_ARG_REQUIRED | MP_ARG_INT, {.u_int = 2} },
        { MP_QSTR_application, MP_ARG_INT, {.u_int = OPUS_APPLICATION_AUDIO} },
        { MP_QSTR_bitrate,     MP_ARG_INT, {.u_int = 64000} },
    };
    mp_arg_val_t vals[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all_kw_array(n_args, n_kw, args, MP_ARRAY_SIZE(allowed_args), allowed_args, vals);

    int sample_rate = vals[ARG_sample_rate].u_int;
    int channels = vals[ARG_channels].u_int;
    int application = vals[ARG_application].u_int;
    int bitrate = vals[ARG_bitrate].u_int;

    int err;
    opusmod_encoder_obj_t *self = m_new_obj(opusmod_encoder_obj_t);
    self->base.type = type;
    self->sample_rate = sample_rate;
    self->channels = channels;
    self->application = application;
    self->bitrate = bitrate;
    self->encoder = opus_encoder_create(sample_rate, channels, application, &err);
    if (!self->encoder || err != OPUS_OK) {
        mp_raise_msg_varg(&mp_type_RuntimeError, "Opus encoder init failed: %d", err);
    }
    if (bitrate > 0) opus_encoder_ctl(self->encoder, OPUS_SET_BITRATE(bitrate));
    return MP_OBJ_FROM_PTR(self);
}

STATIC mp_obj_t opusmod_encoder_encode(mp_obj_t self_in, mp_obj_t pcm_in, mp_obj_t frame_size_in) {
    opusmod_encoder_obj_t *self = MP_OBJ_TO_PTR(self_in);
    mp_buffer_info_t pcminfo;
    mp_get_buffer_raise(pcm_in, &pcminfo, MP_BUFFER_READ);
    int frame_size = mp_obj_get_int(frame_size_in);
    int max_data_bytes = 4000;
    uint8_t *opus_data = m_new(uint8_t, max_data_bytes);

    int nbBytes = opus_encode(self->encoder, (const opus_int16*)pcminfo.buf, frame_size,
                            opus_data, max_data_bytes);
    if (nbBytes < 0) {
        m_del(uint8_t, opus_data, max_data_bytes);
        mp_raise_msg_varg(&mp_type_RuntimeError, "Opus encode failed: %d", nbBytes);
    }
    mp_obj_t result = mp_obj_new_bytes(opus_data, nbBytes);
    m_del(uint8_t, opus_data, max_data_bytes);
    return result;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_3(opusmod_encoder_encode_obj, opusmod_encoder_encode);

STATIC const mp_rom_map_elem_t opusmod_encoder_locals_dict_table[] = {
    { MP_ROM_QSTR(MP_QSTR_encode), MP_ROM_PTR(&opusmod_encoder_encode_obj) },
};
STATIC MP_DEFINE_CONST_DICT(opusmod_encoder_locals_dict, opusmod_encoder_locals_dict_table);

STATIC const mp_obj_type_t opusmod_encoder_type = {
    .base = { &mp_type_type },
    .name = MP_QSTR_OpusEncoder,
    .make_new = opusmod_encoder_make_new,
    .locals_dict = (mp_obj_dict_t*)&opusmod_encoder_locals_dict,
    .del = opusmod_encoder_del,
};

// ----------- 模块定义 -----------

STATIC const mp_rom_map_elem_t opusmod_module_globals_table[] = {
    { MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_opusmod) },
    { MP_ROM_QSTR(MP_QSTR_OpusDecoder), MP_ROM_PTR(&opusmod_decoder_type) },
    { MP_ROM_QSTR(MP_QSTR_OpusEncoder), MP_ROM_PTR(&opusmod_encoder_type) },
    // 可添加更多常量...
};
STATIC MP_DEFINE_CONST_DICT(opusmod_module_globals, opusmod_module_globals_table);

const mp_obj_module_t opusmod_user_cmodule = {
    .base = { &mp_type_module },
    .globals = (mp_obj_dict_t*)&opusmod_module_globals,
};

MP_REGISTER_MODULE(MP_QSTR_opusmod, opusmod_user_cmodule, MODULE_OPUSMOD_ENABLED);
