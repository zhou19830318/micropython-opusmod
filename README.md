# MicroPython Opus C 扩展模块

## 安装

1. 下载最新版官方 Opus 源码（https://opus-codec.org/downloads/），解压到 libopus/。
2. 将本目录作为 user C module 加入 MicroPython 编译（见官方 [C module 文档](https://docs.micropython.org/en/latest/develop/cmodules.html)）。
3. 在 mpconfigport.h 里加入
   #define MODULE_OPUSMOD_ENABLED (1)

## 使用示例

```python
import opusmod

# 编码
encoder = opusmod.OpusEncoder(48000, 2, 2049, 64000)   # 2049=OPUS_APPLICATION_AUDIO
opus_bytes = encoder.encode(pcm_bytes, frame_size)

# 解码
decoder = opusmod.OpusDecoder(48000, 2)
pcm_bytes, samples = decoder.decode(opus_bytes)
