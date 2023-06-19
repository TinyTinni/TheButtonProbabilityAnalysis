#include "NumberExtractor.hpp"
#include <ScreenCapture.h>
#include <Windows.h>
#include <fstream>
#include <iostream>
#include <opencv2/core/persistence.hpp>

void simulate_mouseclick(int x, int y)
{
    INPUT Inputs[3] = { 0 };

    Inputs[0].type = INPUT_MOUSE;
    Inputs[0].mi.dx = x;
    Inputs[0].mi.dy = y;
    Inputs[0].mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE;

    Inputs[1].type = INPUT_MOUSE;
    Inputs[1].mi.dx = x;
    Inputs[1].mi.dy = y;
    Inputs[1].mi.dwFlags = MOUSEEVENTF_LEFTDOWN;

    Inputs[2].type = INPUT_MOUSE;
    Inputs[2].mi.dwFlags = MOUSEEVENTF_LEFTUP;

    SendInput(3, Inputs, sizeof(INPUT));
}

int main()
{
    Analyzer analyzer("cmp/");

    try
    {
        cv::FileStorage fs("options.yml", cv::FileStorage::READ);

        const int RUNS = fs["runs"];
        const cv::Rect ROI(fs["roi"]["x"], fs["roi"]["y"], fs["roi"]["width"], fs["roi"]["height"]);
        const int SCREEN_NUMBER = fs["screen_number"];

        std::ofstream output_file("out.csv");
        output_file << "number\n";

        std::shared_ptr<SL::Screen_Capture::IScreenCaptureManager> mngr;
        std::map<unsigned, unsigned> stats;
        bool can_read_again = true;
        auto grabber = SL::Screen_Capture::CreateCaptureConfiguration([SCREEN_NUMBER]() {return std::vector{ SL::Screen_Capture::GetMonitors()[SCREEN_NUMBER] }; });
        grabber->onNewFrame([&](const SL::Screen_Capture::Image& img, const SL::Screen_Capture::Monitor& window)
            {
                if (!can_read_again)
                {
                    return;
                }
                can_read_again = false;
                static int count = RUNS;
                constexpr int x = 65535 / 2;
                constexpr int y = 65535 / 2;
                cv::Mat image(img.Bounds.bottom, img.Bounds.right, CV_8UC4, (void*)img.Data, img.RowStrideInBytes);
                cv::Mat out = image(ROI);
                cv::resize(out, out, cv::Size(out.size() / 4));
                try
                {
                    auto number = analyzer.number(out);
                    std::cout << "Runs left: " << count << "/" << RUNS << "\n";

                    // check if we are done with all rounds
                    if (number == 0)
                        --count;
                    if (count == 0)
                    {
                        //done, write final results
                        output_file.close();
                        std::ofstream stats_file("stats.csv");
                        stats_file << "number,counts\n";
                        for (auto s : stats)
                        {
                            stats_file << s.first << "," << s.second << "\n";
                        }
                        mngr->pause();
                        std::cout << "done\n";
                        return;
                    }

                    // if not done, write output and click next button
                    std::cout << "number: " << number << "\n";
                    output_file << number << "\n";

                    ++stats[number];
                    simulate_mouseclick(x, y);
                    std::this_thread::sleep_for(std::chrono::milliseconds{300}); //animation is around 300ms long
                    can_read_again = true;

                }
                catch (Analyzer::no_number&)
                {
                }
            });
        mngr = grabber->start_capturing();
        mngr->setFrameChangeInterval(std::chrono::milliseconds{16});
        system("Pause"); // block main thread
    }
    catch (const std::exception& e)
    {
        std::cout << "ERROR: " << e.what();
        return -1;
    }
    return 0;
}