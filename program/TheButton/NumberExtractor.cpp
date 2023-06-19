#include "NumberExtractor.hpp"

cv::Mat padCVMat(const cv::Mat& input,
    const cv::Size& dstSize,
    const cv::Scalar& bgcolor)
{
    cv::Mat output;

    double h1 = dstSize.width * (input.rows / (double)input.cols);
    double w2 = dstSize.height * (input.cols / (double)input.rows);
    if (h1 <= dstSize.height) {
        cv::resize(input, output, cv::Size(dstSize.width, h1));
    }
    else {
        cv::resize(input, output, cv::Size(w2, dstSize.height));
    }

    int top = (dstSize.height - output.rows) / 2;
    int down = (dstSize.height - output.rows + 1) / 2;
    int left = (dstSize.width - output.cols) / 2;
    int right = (dstSize.width - output.cols + 1) / 2;

    cv::copyMakeBorder(output, output, top, down, left, right, cv::BORDER_CONSTANT, bgcolor);

    return output;
}


std::optional<cv::Rect> enclosingRect(cv::Rect lhs, cv::Rect rhs)
{
    if (lhs.contains(rhs.br()) && lhs.contains(rhs.tl()))
        return lhs;
    if (rhs.contains(lhs.br()) && rhs.contains(lhs.tl()))
        return rhs;
    return std::nullopt;
}

uint32_t Analyzer::numberInAreasCmpFunction(const std::vector<cv::Mat>& areas) const
{
    if (areas.empty())
        throw no_number("No number found");

    auto start = std::chrono::high_resolution_clock::now();
    std::vector<uint8_t> p_labels;
    for (const auto& a : areas)
    {
        std::vector<std::pair<uint64_t, size_t>> distances;
        for (const auto& c : m_cmps)
        {
            auto diff = a - c;
            distances.push_back({ (uint64_t)cv::sum(diff)[0], distances.size() });
        }
        p_labels.push_back(std::min_element(distances.begin(), distances.end(), [](const auto& lhs, const auto& rhs) {return lhs.first < rhs.first; })->second);
    }
    std::cout << "number recognized done " << std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - start).count() << "microseconds\n";

    uint32_t number = 0;
    for (uint32_t i = 0; i < p_labels.size(); ++i)
    {
        auto p = p_labels[i];
        number += std::pow(10, i) * p;
    }
    return number;
}

/*uint32_t numberInAreas(const std::vector<cv::Mat>& areas)
{
    std::vector< dlib::matrix<uint8_t>> input;
    for (cv::Mat in : areas)
    {
        input.push_back(dlib::array_to_matrix(dlib::cv_image<uint8_t>(in)));
    }
    //input.push_back(std::move(img));
    auto start = std::chrono::high_resolution_clock::now();
    auto p_labels = m_net(input);
    std::cout << "done " << std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - start).count() << "microseconds\n";
    uint32_t number = 0;
    for (uint32_t i = 0; i < p_labels.size(); ++i)
    {
        auto p = p_labels[i];
        number += std::pow(10, i) * p;
    }
    return number;
}*/

// returns number images of size 28x28 from right to left
std::vector<cv::Mat> Analyzer::numberAreas(cv::Mat input) const {

    std::vector<cv::Mat> bgr;
    cv::split(input, bgr);
    auto red = bgr[2];
    auto start = std::chrono::high_resolution_clock::now();

    cv::Mat threshold_img;
    cv::threshold(red, threshold_img, 240, 255, cv::THRESH_BINARY);
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(threshold_img, contours, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);
    if (contours.empty())
        throw no_number("No number found");
    //remove max contour
    auto biggest_contour = std::max_element(contours.begin(), contours.end(), [](const auto& c, const auto& c2) {return c.size() < c2.size(); });

    contours.erase(biggest_contour);

    std::vector<cv::Rect> boxes;
    for (const auto& c : contours)
        boxes.push_back(cv::boundingRect(c));

    std::vector<cv::Rect> outerContours;
    for (int i = 0; i < boxes.size(); ++i)
    {
        std::optional<cv::Rect> r;
        int j = 0;
        if (boxes[i].width < 10 || boxes[i].height < 10)
            continue;
        for (; j < outerContours.size() && !r.has_value(); ++j)
        {
            r = enclosingRect(boxes[i], outerContours[j]);
        }
        if (r)
            outerContours[j - 1] = *r;
        else
            outerContours.push_back(boxes[i]);
    }

    // sort contours from right to left
    std::sort(outerContours.begin(), outerContours.end(), [](const cv::Rect& lhs, const cv::Rect& rhs) {return lhs.x > rhs.x; });

    std::vector<cv::Mat> numbers_right_to_left;
    for (auto m : outerContours)
    {
        cv::Mat end_result = threshold_img(m);
        end_result = padCVMat(end_result, cv::Size(28, 28), 255);
        end_result = 255 - end_result;
        numbers_right_to_left.push_back(end_result);
    }
    std::cout << "number extration done " << std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - start).count() << "microseconds\n";
    return numbers_right_to_left;
}
