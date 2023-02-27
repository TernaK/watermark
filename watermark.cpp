#include "watermark.h"

Watermark::Watermark(std::string logo_path, float logo_scale, int output_size, cv::Point offset)
: logo_scale_(logo_scale), output_size_(output_size), offset_(offset)
{
    if(file_is_png(logo_path)) {
        load_png_logo(logo_path);
    } else {
        throw std::runtime_error("Only PNG logos supported!");
    }
}

std::vector<std::string> Watermark::get_jpegs_in_folder(std::string folder)
{
    std::vector<std::string> jpegs;

    std::filesystem::path folder_path(folder);
    for (auto const& dir_entry : std::filesystem::directory_iterator(folder_path)) {
        std::string extension = dir_entry.path().extension();
        if (dir_entry.is_regular_file() && extension == ".jpg") {
            jpegs.push_back(std::string(dir_entry.path()));
        }
    }

    return jpegs;
}

cv::Mat Watermark::load_png_logo(std::string path)
{
    cv::Mat image = cv::imread(path, cv::IMREAD_UNCHANGED);
    if (image.channels() != 4)
        throw std::runtime_error("Transparent PNG logo required!");

    cv::resize(image, image, cv::Size((float)image.cols * logo_scale_,
                                      (float)image.rows * logo_scale_));

    std::vector<cv::Mat> channels;
    cv::split(image, channels);

    cv::merge(std::vector<cv::Mat>{channels[0], channels[1], channels[2]}, logo_image_);
    logo_mask_ = channels[3];

    channels[3].convertTo(logo_mask_, CV_32F, 1.0f/255.0f);
    logo_inv_mask_ = 1.0 - logo_mask_;

    return image;
}

cv::Mat Watermark::load_jpeg_logo(std::string path)
{
    return {};
}

bool Watermark::file_is_jpeg(std::string file_path)
{
    std::string extension = std::filesystem::path(file_path).extension().string();
    std::transform(extension.begin(), extension.end(), extension.begin(), [](char c) -> char {
        return std::tolower(c);
    });
    return std::set<std::string>{".jpg", ".jpeg"}.count(extension) > 0;
}

bool Watermark::file_is_png(std::string file_path)
{
    std::string extension = std::filesystem::path(file_path).extension().string();
    std::transform(extension.begin(), extension.end(), extension.begin(), [](char c) -> char {
        return std::tolower(c);
    });
    return std::set<std::string>{".png"}.count(extension) > 0;
}

cv::Mat Watermark::aspect_resize(cv::Mat const& image)
{
    int new_width;
    int new_height;

    if (image.rows >= image.cols) {
        new_height = output_size_;
        new_width = image.cols * new_height / image.rows;
    } else {
        new_width = output_size_;
        new_height = image.rows * new_width / image.cols;
    }
    
    cv::Mat output;
    cv::resize(image, output, {new_width, new_height}, cv::INTER_LANCZOS4);
    return output;
}

cv::Mat Watermark::aspect_fill(cv::Mat const& image)
{
    int offset_x = 0;
    int offset_y = 0;

    if (image.rows >= image.cols) {
        offset_x = (output_size_ - image.cols) / 2;
    } else {
        offset_y = (output_size_ - image.rows) / 2;
    }

    cv::Mat output = cv::Mat::zeros(output_size_, output_size_, CV_8UC3);
    image.copyTo(output(cv::Rect(offset_x, offset_y, image.cols, image.rows)));
    return output;
}

void Watermark::blend_logo(cv::Mat& image)
{
    cv::Rect logo_pos {
        offset_.x,
        image.rows - offset_.y - logo_image_.rows,
        logo_image_.cols,
        logo_image_.rows
    };

    cv::Mat patch = image(logo_pos);
    cv::blendLinear(patch, logo_image_, logo_inv_mask_, logo_mask_, patch);
}

void Watermark::operator()(cv::Mat& image)
{
    if (image.cols < output_size_)
        std::cerr << "WARNING: Image width < " << output_size_ << ": Upsampling..." << std::endl;
    if (image.rows < output_size_)
        std::cerr << "WARNING: Image height < " << output_size_ << ": Upsampling..." << std::endl;

    // resize image
    image = aspect_resize(image);

    // blend-in logo
    blend_logo(image);

    // pad for non-square images
    image = aspect_fill(image);
}
