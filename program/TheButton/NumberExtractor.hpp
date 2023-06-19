#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>

#include <iostream>

#include <chrono>
#include <ranges>
#include <optional>

class Analyzer
{
    std::vector<cv::Mat> m_cmps;

public:
    class no_number : public std::runtime_error
    {
    public:
        using std::runtime_error::runtime_error;
    };

    Analyzer(std::string_view cmp_path) {
        for (int i = 0; i < 10; ++i)
        {
            m_cmps.push_back(cv::imread(std::string(cmp_path) + std::to_string(i) + ".png", cv::IMREAD_GRAYSCALE));
        }
    }

    uint32_t number(std::string_view image_path) const
    {
        auto original = cv::imread(std::string(image_path));
        if (original.empty())
            throw std::runtime_error("Cannot load image " + std::string(image_path));
        return number(original);
    }

    uint32_t number(cv::Mat mat) const
    {
        return numberInAreasCmpFunction(numberAreas(mat));
    }

    void saveExtractedImage(std::string_view image_path, std::string_view out_path) const
    {
        auto original = cv::imread(std::string(image_path));
        if (original.empty())
            throw std::runtime_error("Cannot load image " + std::string(image_path));
        auto imgs = numberAreas(original);
        if (imgs.size() != 1)
            throw std::runtime_error("More than 1 number in the image");
        cv::imwrite(std::string(out_path), imgs[0]);
    }

private:
    // returns the number on the given 28x28 images. One image contains a single digit
    uint32_t numberInAreasCmpFunction(const std::vector<cv::Mat>& areas) const;

    // returns the number images of size 28x28 from right to left. Each image contains a single digit
    std::vector<cv::Mat> numberAreas(cv::Mat input) const;
};