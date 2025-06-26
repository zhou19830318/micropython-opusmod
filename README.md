# MicroPython Opus C 扩展模块

This module provides MicroPython bindings to the Opus audio codec.

## 源码结构

The core Opus C source files required for this module are **not** included directly in this repository due to their large number, which can exceed limits in some development environments or when checking into version control.

This repository provides:
- The MicroPython C module bindings (`opusmod.c`).
- Build files (`micropython.mk`, `micropython.cmake`) configured to compile the Opus sources.
- A partial set of Opus public API headers in `libopus/include/`.
- A partial set of core Opus source files in `libopus/src/`.
- The directory structure required for you to place the remaining Opus library files:
  - `libopus/src/celt/`
  - `libopus/src/silk/`
  - `libopus/src/silk/fixed/`
  - `libopus/src/silk/float/`

## 安装

To compile this module, you first need to obtain the Opus source code and place the necessary files into this repository structure.

1.  **Download Opus Source Code:**
    Download Opus version 1.5.2 source code from the official website:
    [https://downloads.xiph.org/releases/opus/opus-1.5.2.tar.gz](https://downloads.xiph.org/releases/opus/opus-1.5.2.tar.gz)

2.  **Extract Required Opus Files:**
    Extract the downloaded `opus-1.5.2.tar.gz` archive. You will need to copy specific files from the `opus-1.5.2` directory into the `libopus/src/` directory of this module as follows:

    *   **CELT Sources & Headers:**
        *   Copy the following C files from `opus-1.5.2/celt/` to `libopus/src/celt/`:
            `bands.c, celt.c, celt_decoder.c, celt_encoder.c, celt_lpc.c, cwrs.c, entcode.c, entdec.c, entenc.c, kiss_fft.c, laplace.c, mathops.c, mdct.c, modes.c, pitch.c, quant_bands.c, rate.c, vq.c`
        *   Copy all header files (`*.h`) from the top-level of `opus-1.5.2/celt/` to `libopus/src/celt/`.
    *   **SILK Sources & Headers (Main):**
        *   Copy the following C files from `opus-1.5.2/silk/` to `libopus/src/silk/`:
            `A2NLSF.c, CNG.c, LPC_analysis_filter.c, LPC_fit.c, LPC_inv_pred_gain.c, LP_variable_cutoff.c, NLSF2A.c, NLSF_VQ.c, NLSF_VQ_weights_laroia.c, NLSF_decode.c, NLSF_del_dec_quant.c, NLSF_encode.c, NLSF_stabilize.c, NLSF_unpack.c, NSQ.c, NSQ_del_dec.c, PLC.c, VQ_WMat_EC.c, ana_filt_bank_1.c, biquad_alt.c, bwexpander.c, bwexpander_32.c, check_control_input.c, code_signs.c, control_SNR.c, control_audio_bandwidth.c, control_codec.c, debug.c, dec_API.c, decode_core.c, decode_frame.c, decode_indices.c, decode_parameters.c, decode_pitch.c, decode_pulses.c, decoder_set_fs.c, enc_API.c, encode_indices.c, encode_pulses.c, gain_quant.c, HP_variable_cutoff.c, init_decoder.c, init_encoder.c, inner_prod_aligned.c, interpolate.c, lin2log.c, log2lin.c, pitch_est_tables.c, process_NLSFs.c, quant_LTP_gains.c, resampler.c, resampler_down2.c, resampler_down2_3.c, resampler_private_AR2.c, resampler_private_IIR_FIR.c, resampler_private_down_FIR.c, resampler_private_up2_HQ.c, resampler_rom.c, shell_coder.c, sigm_Q15.c, sort.c, stereo_LR_to_MS.c, stereo_MS_to_LR.c, stereo_decode_pred.c, stereo_encode_pred.c, stereo_find_predictor.c, stereo_quant_pred.c, sum_sqr_shift.c, table_LSF_cos.c, tables_LTP.c, tables_NLSF_CB_NB_MB.c, tables_NLSF_CB_WB.c, tables_gain.c, tables_other.c, tables_pitch_lag.c, tables_pulses_per_block.c, VAD.c`
        *   Copy all header files (`*.h`) from the top-level of `opus-1.5.2/silk/` to `libopus/src/silk/`.
    *   **SILK Fixed-Point Sources & Headers:**
        *   Copy the following C files from `opus-1.5.2/silk/fixed/` to `libopus/src/silk/fixed/`:
            `LTP_analysis_filter_FIX.c, LTP_scale_ctrl_FIX.c, apply_sine_window_FIX.c, autocorr_FIX.c, burg_modified_FIX.c, corrMatrix_FIX.c, encode_frame_FIX.c, find_LPC_FIX.c, find_LTP_FIX.c, find_pitch_lags_FIX.c, find_pred_coefs_FIX.c, k2a_FIX.c, k2a_Q16_FIX.c, noise_shape_analysis_FIX.c, pitch_analysis_core_FIX.c, process_gains_FIX.c, regularize_correlations_FIX.c, residual_energy16_FIX.c, residual_energy_FIX.c, schur64_FIX.c, schur_FIX.c, vector_ops_FIX.c, warped_autocorrelation_FIX.c`
        *   Copy all header files (`*.h`) from the top-level of `opus-1.5.2/silk/fixed/` to `libopus/src/silk/fixed/`.
    *   **SILK Float-Point Sources & Headers:**
        *   Copy the following C files from `opus-1.5.2/silk/float/` to `libopus/src/silk/float/`:
            `LPC_analysis_filter_FLP.c, LPC_inv_pred_gain_FLP.c, LTP_analysis_filter_FLP.c, LTP_scale_ctrl_FLP.c, apply_sine_window_FLP.c, autocorrelation_FLP.c, burg_modified_FLP.c, bwexpander_FLP.c, corrMatrix_FLP.c, encode_frame_FLP.c, energy_FLP.c, find_LPC_FLP.c, find_LTP_FLP.c, find_pitch_lags_FLP.c, find_pred_coefs_FLP.c, inner_product_FLP.c, k2a_FLP.c, noise_shape_analysis_FLP.c, pitch_analysis_core_FLP.c, process_gains_FLP.c, regularize_correlations_FLP.c, residual_energy_FLP.c, scale_copy_vector_FLP.c, scale_vector_FLP.c, schur_FLP.c, sort_FLP.c, warped_autocorrelation_FLP.c, wrappers_FLP.c`
        *   Copy all header files (`*.h`) from the top-level of `opus-1.5.2/silk/float/` to `libopus/src/silk/float/`.

    *Note: The public API headers in `libopus/include/` and core Opus files like `opus.c`, `opus_encoder.c`, `opus_decoder.c`, `opus_private.h` in `libopus/src/` are already provided in this repository and should be compatible with opus-1.5.2.*

3.  **MicroPython Build:**
    Add this directory as a user C module to your MicroPython build. Refer to the official MicroPython documentation on [User C Modules](https://docs.micropython.org/en/latest/develop/cmodules.html).
    You will typically need to set `USER_C_MODULES` in your make command or CMake configuration to point to the directory containing this `opusmod` module.

4.  **Enable in `mpconfigport.h`:**
    Add the following line to your port's `mpconfigport.h` file:
    ```c
    #define MODULE_OPUSMOD_ENABLED (1)
    ```

5.  **Rebuild MicroPython.**

## 使用示例

```python
import opusmod

# 编码
# OpusEncoder(sampling_rate, channels, application, bitrate_bps)
# application: 2048 (VoIP), 2049 (Audio), 2051 (Restricted Lowdelay)
encoder = opusmod.OpusEncoder(48000, 2, 2049, 64000)
# pcm_bytes: bytes object containing raw PCM audio data (16-bit signed little-endian)
# frame_size: number of samples per channel in pcm_bytes
opus_bytes = encoder.encode(pcm_bytes, frame_size)

# 解码
# OpusDecoder(sampling_rate, channels)
decoder = opusmod.OpusDecoder(48000, 2)
# opus_bytes: bytes object containing Opus encoded audio data
# Returns a tuple: (decoded_pcm_bytes, number_of_samples_decoded_per_channel)
pcm_bytes, samples = decoder.decode(opus_bytes)
```
