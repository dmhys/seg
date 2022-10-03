#ifndef SEG_OBJECT_IMAGE_H
#define SEG_OBJECT_IMAGE_H

#include <atomic>
#include <mutex>
#include <string>
#include <vector>

#include "seg/object/ui_object.h"
#include "seg/types.h"

#ifdef OPENCV
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#endif

typedef unsigned int GLuint;

namespace seg {
namespace object {
class Image : public UIObject
{
   public:
    Image(std::string name);

    void setData(void* image_buffer, ImageSize size, ImageType type);

    const std::string getType() const override { return "Image"; }

   private:
    void drawImpl() override;
    void updateTexture();

    std::string name;

    std::mutex mtx;
    std::atomic<bool> updated{false};
    std::vector<unsigned char> buffer;  // TODO:
    GLuint texture_id = 0;
    int width;
    int height;

#ifdef OPENCV
   public:
    void setData(cv::Mat&& image);
    void setData(const cv::Mat& image);
    cv::Mat image_buffer;
#endif

};  // class Image
}  // namespace object
}  // namespace seg

#endif