#pragma once
#include <opencv2/opencv.hpp>
#include <iostream>
#include <filesystem>

class Watermark
{
public:
    Watermark(std::string logo_path, float logo_scale, int output_size = 2160, cv::Point offset = {150, 75});

    void operator()(cv::Mat& image);

    static std::vector<std::string> get_jpegs_in_folder(std::string folder);

private:
    static bool file_is_jpeg(std::string file_path);

    static bool file_is_png(std::string file_path);

    cv::Mat aspect_resize(cv::Mat const& image);

    cv::Mat aspect_fill(cv::Mat const& image);

    cv::Mat load_png_logo(std::string path);

    cv::Mat load_jpeg_logo(std::string path);

    void blend_logo(cv::Mat& image);

    cv::Mat logo_image_;
    cv::Mat logo_mask_;
    cv::Mat logo_inv_mask_;
    float logo_scale_;
    int output_size_;
    cv::Point offset_;
};
