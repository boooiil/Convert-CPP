```ts

let output = { "Runner": Runner }

interface Runner {
   converting: MediaFile[],
   pending: MediaFile[]

}

interface MediaFile {
   // https://github.com/boooiil/Convert-CPP/blob/main/src/application/Activity.cpp
   activity: ActivityType, 
   cwd: string,
   // original name of the file
   name: string,
   // epoch of when the conversion started
   started: number,
   // epoch of when the conversion ended
   ended: number, 
   ffmpegArguments: string[],
   file: {
      // absolute path to the converted file
      conversionFilePath: string, 
      conversionFolderPath: string,
      // name of the converted file without extension
      conversionName: string, 
      // name of the converted file with extension
      conversionNameExt: string, 
      ext: string,
      // converted file size
      newSize: number,
      // name of the original file with extension
      originalFileNameExt: string,
      // absolute path to the original file
      originalFilePath: string,
      // quality of the file if provided in the filename
      quality: number,
      // season number if provided in the filename
      season: number,
      // series name
      series: string,
      // original file size
      size: number,
      // validation size (should be the same as newSize)
      validationSize: number
   },
   video: {
      // converted video height
      convertedHeight: string,
      // converted video resolution (width:height)
      convertedResolution: string,
      // converted video width
      convertedWidth: string,
      // MediaFormat crf value (See Resolutions in README.md)
      crf: number,
      // crop of the video
      crop: string,
      // frames per second of the video
      fps: number,
      // original video height
      height: number,
      // original video resolution (width:height)
      ratio: string,
      // subtitle provider (defunct)
      subtitleProvider: string,
      // total frames in the video
      totalFrames: number,
      // original video width
      width: number
   },
   working: {
      bitrate: number,
      completedFrames: number,
      // encoded frames per second
      fps: number,
      quality: number
   }
}

```