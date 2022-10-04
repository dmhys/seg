#include <vector>
#include <string>
#include <thread>

#include "seg/seg"
#include "seg/object/primitives.h"
#include "seg/utilities/filesystem.h"

#include <opencv2/opencv.hpp>

using namespace std::literals::chrono_literals;
namespace segobj = ::seg::object;

class ToyVo
{
public:
    ToyVo()
    {
        feature_view = std::make_shared<segobj::Image>("Feature");
        seg::addObject("Feature View", feature_view);

        track_view  = std::make_shared<segobj::Image>("Track");
        seg::addObject("Track View", track_view);

        odom_view = std::make_shared<segobj::Pose>();
        seg::addObject("Odometry", odom_view);

        path_view = std::make_shared<segobj::Path>();
        seg::addObject("Trajectory", path_view);

        tracked_cloud_view = std::make_shared<segobj::Pointcloud>();
        seg::addObject("Tracked Features", tracked_cloud_view);
    }

    void reset()
    {
        is_initialized = false;
    }

    void setNewFrame(cv::Mat image)
    {
        if(feature_view->is_visible)
            feature_view->setData(image);
        
        if(track_view->is_visible)
            track_view->setData(image);

        if(tracked_cloud_view->is_visible)
            tracked_cloud_view->addData(segobj::primitives::GaussianRandomVertices(1,3)[0]);

    }

private:
    bool is_initialized = false;
    std::shared_ptr<segobj::Image> feature_view;
    std::shared_ptr<segobj::Image> track_view;

    std::shared_ptr<segobj::Pose> odom_view;
    std::shared_ptr<segobj::Path> path_view;

    std::shared_ptr<segobj::Pointcloud> tracked_cloud_view;
};

void test()
{
    auto folder = std::string("/home/hyun/datasets/V1_01_easy/mav0/cam0/data/");
    auto filelist = seg::getFileList(folder,".png");

    cv::Ptr<cv::ORB> detector = cv::ORB::create(1500);
    cv::Ptr<cv::DescriptorMatcher> matcher =
        cv::DescriptorMatcher::create(cv::DescriptorMatcher::BRUTEFORCE_HAMMINGLUT);

    cv::Mat img1 = cv::imread(folder+filelist[180]);
    cv::Mat img2 = cv::imread(folder+filelist[200]);

    std::vector<cv::KeyPoint> keypts1;
    std::vector<cv::KeyPoint> keypts2;
    cv::Mat descriptors1, descriptors2;

    detector->detectAndCompute(img1, cv::noArray(), keypts1, descriptors1);
    detector->detectAndCompute(img2, cv::noArray(), keypts2, descriptors2);

    cv::Mat result;
    std::vector< cv::DMatch > matches;
    matcher->match( descriptors1, descriptors2, matches );
    drawMatches( img1, keypts1, img2, keypts2, matches, result );

    // cv::Mat Essential = cv::findEssentialMat(keypts1,keypts2);

    cv::imshow("mat",result);
    cv::waitKey();

}

int segcall()
{
    seg::initialize("Visual Odometry Example");

    ToyVo engine;

    auto folder = std::string("/home/hyun/datasets/V1_01_easy/mav0/cam0/data/");
    auto filelist = seg::getFileList(folder,".png");

    for(auto && file:filelist)
    {
        cv::Mat mat = cv::imread(folder+file);
        engine.setNewFrame(mat);

        std::this_thread::sleep_for(33ms);
    }

    return 0;
}

int main()
{
    segcall();
    
    return 0;
}
