# Convert
Re-encodes all MKV, MP4 and AVI files in a root directory to MKV files using FFmpeg utilizing hardware acceleration if specified. Defaults are extremely conservative for bitrate. A search string will be used to determine the folder for the output files. 



The conversion process will place files differently based on the search string found in [MediaFile.cpp](/src/program/child/media/MediaFile.cpp). This has three cases:
* The included media are of the same season and series.
> The media will be placed in a folder such as: `{CWD}/{Series Name} Season {Season Number}/`. The resulting filename will be: `{Series Name} - s##e##.mkv` with a fill path of `{CWD}/{Series Name} Season {Season Number}/{Series Name} - s##e##.mkv`.
* The included media are of mixed seasons and or series.
> The media will be placed in a different folder based on its matched series and season. This can be impacted by mixed case series names. For example: `example series` would result in a different folder than `Example Series`. This will be resolved in later iterations.
> This would look something like:
>
> Input: `example.series.s01e01.mkv, Example.Series.S01E02.mkv`
>
> 1: `{CWD}/example series Season 1/example series - s01e01.mkv`
>
> 2: `{CWD}/Example Series Season 1/Example Series - s01e02.mkv`
* The included media did not match the search string.
> The media will be placed in a folder named `converted` with the original file name intact.
> 
> Input: `some.unknown.file.name.mkv`
>
> Output: `{CWD}/converted/some.unknown.file.name.mkv`

## Usage
```
./convert [...args]
```
## Options
```
-a, --amount <number>
   Set the amont of files to convert at once. (Default: 1)

-ac, --audiochannels <num1,num2,...>
   Specify number of audio channels for each audio stream.
   (Will default to original channel count if not specified)

-af, --audioformat <string1,string2,...>
   Specify the audio codec for each audio stream.
   (Will default to original codec if not specified)

-as, --audiostreams <num1,num2,...>
   Specify which audio streams to keep. (Default: All)

-b, --bitrate
    Use bitrates instead of CRF. You can only use defined resolutions with this flag.

-c, --crop
   Crop the media to the specified resolution.

-co, --constrain
    Force the encoder to use a max bitrate with CRF.

-crf, --crf <number>
   Override the CRF value for the current media.

-dr, --displayrefresh <number>
   Override the display output refresh rate in ms. (Default: 1000)

-e, --encoder <string>
   One of the pre-configured encoders

-h, --help
   Display this help message.

-hwd, --hardwaredecode
   Use hardware decoding.

-hwe, --hardwareencode
   Use hardware encoding.

-i, --info
   Display information about the media or child processes.

-lf, --loggingformat <string>
   Specify the logging format. (Default: Default)

-o, --overwrite
   Overwrite existing media if it exists in the conversion path.

-parent
   Run the program as a parent process. (Default: False)

-q, --quality <number>p
   Resolution of the media to convert.

-s, --start <hh:mm:ss>
   Skip the beginning by specified amount of time.

-t, --tune <string>
   One of the ffmpeg tune profiles

-tr, --trim <hh:mm:ss,hh:mm:ss>
   Trim the media.

```

## Logging Formats
```
   Default:
         Head: [TIME] [TARGET ENC] [ENC] [ACC] [RES] [TUNE] [AMOUNT] [?CROP]
         File: [FILE] [ACT] [START] [PROG] [QUAL] [BITRATE] [SPEED] [ETA]
         ...

   JSON:
         (See JSONFormat.md)
```

## Resolutions
```
   Custom: 
     <value>p
        CRF: 24
        WIDTH: CALCULATED
        HEIGHT: PROVIDED
        CROP: CALCULATED (2.40:1 ~ 21:9)

   Configured: 
     2160p
        CRF: 24
        BITRATE: 30M
        MIN: 30M
        MAX: 40M
        WIDTH: 3840
        HEIGHT: 2160
        CROP: 3840:1600 (2.40:1 ~ 21:9)
        
     1440p
        CRF: 24
        BITRATE: 20M
        MIN: 20M
        MAX: 27M
        WIDTH: 2560
        HEIGHT: 1440
        CROP: 2560:1068 (2.40:1 ~ 21:9)

     1080p
        CRF: 24
        BITRATE: 2M
        MIN: 1.6M
        MAX: 2.2M
        WIDTH: 1920
        HEIGHT: 1080
        CROP: 1920:800 (2.40:1 ~ 21:9)

     1080pn (Traditional TV/Netflix Cropping)
        ...SAME AS 1080p...
        CROP: 1920:960 (2:1)

     1080pm (Some One-off Marvel Cropping)
        ...SAME AS 1080p...
        CROP: 1920:870 (64:29)

     720p
        CRF: 24
        BITRATE: 1.4M
        MIN: 1.2M
        MAX: 1.8M
        WIDTH: 1280
        HEIGHT: 720
        CROP: 1280:534 (2.40:1 ~ 21:9)

     720pn (Traditional TV/Netflix Cropping)
        ...SAME AS 720p...
        CROP: 1280:640 (2:1)

     720pm (Some One-off Marvel Cropping)
        ...SAME AS 720p...
        CROP: 1280:580 (64:29)

     480p
        CRF: 24
        BITRATE: 0.6M
        MIN: 0.4M
        MAX: 0.8M
        WIDTH: 854
        HEIGHT: 480
        CROP: 854:356 (2.40:1 ~ 21:9)

     480pc (NTSC Cropping)
        ...SAME AS 480p...
        WIDTH: 1138
        HEIGHT: 640
        CROP: 854:720 (32:27 ~ 4:3)
```
## Encoders
```
   NVIDIA:
        AV1_NVENC (RTX 4000 OR NEWER)
        HEVC_NVENC (GTX 600 OR NEWER)
        H264_NVENC
    
    AMD:
        AV1_AMF (RX 7000 OR NEWER)
        HEVC_AMF (CARRIZO/RX 480 OR NEWER)
        H264_AMF

    Intel:
        AV1_QSV (ARC 300 OR NEWER)
        HEVC_QSV (HD 500/ARC 300 OR NEWER)
        H264_QSV

    Software:
        AV1 (NOT RECOMMENDED)
        HEVC (NEWER i5/R5 RECOMMENDED)
        H264
```
## Hardware Accelerators
```
   NVIDIA:
        CUDA (GPU MUST SUPPORT INPUT VIDEO CODEC)

    AMD:
        AMF (APU/GPU MUST SUPPORT INPUT VIDEO CODEC)

    Intel:
        QSV (IGPU/GPU MUST SUPPORT INPUT VIDEO CODEC)

    Unused:
        VULKAN (NOT TESTED)
```
