#include "watermark.h"

std::string const ASSETS_FOLDER = std::string(ASSETS_DIR);

int main(int argc, char *args[])
{
    std::string photos_folder = "/Users/terna/Downloads/RNS";
    std::string output_folder = "/Users/terna/Downloads/RNS/RNS_watermarked";
    std::string logo_path = ASSETS_FOLDER + "/RNS_logo.png";

    float logo_scale = 1.0;

    Watermark watermark(logo_path, logo_scale);

    auto jpegs = Watermark::get_jpegs_in_folder(photos_folder);

    std::filesystem::create_directory(std::filesystem::path(output_folder));

    for (auto jpeg : jpegs) {
        // load image
        cv::Mat image = cv::imread(jpeg);

        std::cerr << "Processing image: " << jpeg << " -> " << image.size() << std::endl;
        watermark(image);

        // save image
        std::filesystem::path output_path = output_folder / std::filesystem::path(jpeg).filename();
        cv::imwrite(output_path.string(), image);
        std::cerr << "Saved image: " << jpeg << " -> " << image.size() << std::endl;
    }

    return 0;

}

