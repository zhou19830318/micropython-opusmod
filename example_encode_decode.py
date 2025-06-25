import opusmod

# 假设你有一段 int16 小端格式的 PCM 数据
# 示例：生成1秒的1kHz单声道正弦波，采样率48000
import array, math
sample_rate = 48000
frame_size = 960
channels = 1
duration_sec = 1
pcm = array.array('h', [
    int(30000 * math.sin(2 * math.pi * 1000 * t / sample_rate))
    for t in range(sample_rate * duration_sec)
])
pcm_bytes = pcm.tobytes()

# 创建 Opus 编码器
encoder = opusmod.OpusEncoder(sample_rate, channels, 2049, 64000)  # 2049=OPUS_APPLICATION_AUDIO

# 分帧编码（Opus 推荐帧长960/1920/2880等）
opus_frames = []
for i in range(0, len(pcm), frame_size):
    frame = pcm[i:i+frame_size]
    if len(frame) < frame_size:
        # 补零
        frame.extend([0] * (frame_size - len(frame)))
    opus_data = encoder.encode(frame.tobytes(), frame_size)
    opus_frames.append(opus_data)

# opus_frames 现在是多个opus编码包

import opusmod

sample_rate = 48000
channels = 1

# 创建解码器
decoder = opusmod.OpusDecoder(sample_rate, channels)

# 假设 opus_bytes 是你要解码的 Opus 数据包
# 这里解码上面编码得到的 opus_frames
pcm_out = bytearray()
for opus_bytes in opus_frames:
    pcm_bytes, samples = decoder.decode(opus_bytes)
    pcm_out.extend(pcm_bytes)
    print("解码帧样本数:", samples)

# pcm_out 现在是连续的 int16 PCM 数据，可直接播放或保存
