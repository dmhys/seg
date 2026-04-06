#pragma once

#include <cstdio>
#include <functional>
#include <memory>
#include <string>
#include <type_traits>
#include <vector>

namespace seg {
namespace ui {

class GeneralInspector {
 public:
  static const int DEFAULT_ITEM_WIDTH = 150;

 private:
  struct Field {
    std::string name;
    std::function<std::string()> display;
  };

  template <typename T>
  static std::string format(T* pointer) {
    if constexpr (std::is_same_v<T, bool>) {
      return *pointer ? "True" : "False";
    } else if constexpr (std::is_same_v<T, std::string>) {
      return *pointer;
    } else if constexpr (std::is_same_v<T, float> ||
                         std::is_same_v<T, double>) {
      char buf[32];
      std::snprintf(buf, sizeof(buf), "%.1f", static_cast<double>(*pointer));
      return buf;
    } else if constexpr (std::is_integral_v<T>) {
      return std::to_string(*pointer);
    } else {
      return "unsupported";
    }
  }

 public:
  class Builder {
   public:
    template <typename T>
    Builder& addField(std::string field_name, T* pointer) {
      field_data.push_back({std::move(field_name), [pointer] {
                              return GeneralInspector::format(pointer);
                            }});
      return *this;
    }

    template <typename T>
    Builder& addField(std::string field_name, const T* pointer) {
      return addField(field_name, const_cast<T*>(pointer));
    }

    Builder& addDrawFunction(std::function<void()> draw_function) {
      function_data.push_back(std::move(draw_function));
      return *this;
    }

    std::unique_ptr<GeneralInspector> build() {
      return std::unique_ptr<GeneralInspector>(new GeneralInspector(
          std::move(field_data), std::move(function_data)));
    }

   private:
    std::vector<Field> field_data;
    std::vector<std::function<void()>> function_data;
  };  // class GeneralInspector::Builder

  void draw();

 private:
  GeneralInspector() = delete;
  GeneralInspector(std::vector<Field>&& field_data,
                   std::vector<std::function<void()>>&& function_data)
      : fields(field_data), functions(function_data) {}

  void drawFields();
  void drawFunctions();

  std::vector<Field> fields;
  std::vector<std::function<void()>> functions;

};  // class GeneralInspector
}  // namespace ui
}  // namespace seg
