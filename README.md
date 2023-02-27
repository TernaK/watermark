# watermark

Apply an alpha-mask PNG watermark to a batch of JPEG images.

## Prerequisites

- OpenCV
- XCode

## Usage

```
mkdir build
cd build
cmake -GXcode ..
open watermark.xcodeproj
```

Edit `main.cpp` and set the following:

- `photos_folder`: Path to directory containing JPEG photos to watermark.
- `output_folder`: Path to output directory where watermarked photos will be stored.
- `logo_path` - Path to PNG watermark image with transparency/alpha layer.