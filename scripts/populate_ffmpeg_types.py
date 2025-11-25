import os
import sys
from ffmpeg_codecs import audio_codec, container, video_codec, subtitle_codec, factory
# import audio_codec
# import container
# import video_codec
# import subtitle_codec
# import factory

#print("CWD:", os.getcwd())
#print("args:", sys.argv)

root: str = sys.argv[1]
ffmpeg_path = os.path.join(root, "src", "program", "child", "ffmpeg")
data_path = os.path.join(root, "scripts", "data", "ffmpeg_codecs")

# Generate Audio Codecs
audioCodecs = audio_codec.generate(
    os.path.join(data_path, "audio_codecs.json"), 
    os.path.join(ffmpeg_path, "audio"))

# Generate Containers
containers = container.generate(
    os.path.join(data_path, "containers.json"), 
    os.path.join(ffmpeg_path, "container"))

# Generate Video Codecs
videoCodecs = video_codec.generate(
    os.path.join(data_path, "video_codecs.json"), 
    os.path.join(ffmpeg_path, "video"))

# Generate Subtitle Codecs
subtitleCodecs = subtitle_codec.generate(
    os.path.join(data_path, "subtitle_codecs.json"), 
    os.path.join(ffmpeg_path, "subtitle"))

# Generate Audio Codec Factory
factory.generate(
    "AudioCodec", 
    "BaseAudioCodec", 
    list(audioCodecs.keys()), 
    os.path.join(ffmpeg_path, "audio")
)

# Generate Container Factory
factory.generate(
    "Container", 
    "BaseContainer", 
    list(containers.keys()), 
    os.path.join(ffmpeg_path, "container")
)

# Generate Video Codec Factory
factory.generate(
    "VideoCodec", 
    "BaseVideoCodec", 
    list(videoCodecs.keys()), 
    os.path.join(ffmpeg_path, "video")
)

# Generate Subtitle Codec Factory
factory.generate(
    "SubtitleCodec", 
    "BaseSubtitleCodec", 
    list(subtitleCodecs.keys()), 
    os.path.join(ffmpeg_path, "subtitle")
)

print("Types OK.")