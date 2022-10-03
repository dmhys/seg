#ifndef SEG_UI_GENERAL_INSPECTOR_H
#define SEG_UI_GENERAL_INSPECTOR_H

#include <functional>
#include <memory>
#include <string>
#include <tuple>
#include <typeinfo>
#include <vector>

namespace seg {
namespace ui {

class GeneralInspector
{
   public:
    static const int DEFAULT_ITEM_WIDTH = 150;

   private:
#define MACRO_LINK_MEMBER(ID, NAME) \
    decltype(std::get<ID>(data)) NAME() { return std::get<ID>(data); }
    struct Field
    {
        std::tuple<std::string, void*, const std::type_info*> data;
        MACRO_LINK_MEMBER(0, name);
        MACRO_LINK_MEMBER(1, pointer);
        MACRO_LINK_MEMBER(2, type);
        Field(std::string _string, void* ptr, const std::type_info* typeinfo)
            : data(std::make_tuple(_string, ptr, typeinfo))
        {
        }
    };
#undef MACRO_LINK_MEMBER

   public:
    class Builder
    {
       public:
        template <typename T>
        Builder& addField(std::string field_name, T* pointer)
        {
            field_data.push_back(std::move(
                Field(field_name, static_cast<void*>(pointer), &typeid(pointer))));
            return *this;
        }

        template <typename T>
        Builder& addField(std::string field_name, const T* pointer)
        {
            return addField(field_name, const_cast<T*>(pointer));
        }

        Builder& addDrawFunction(std::function<void()> draw_function)
        {
            function_data.push_back(std::move(draw_function));
            return *this;
        }

        std::unique_ptr<GeneralInspector> build()
        {
            return std::unique_ptr<GeneralInspector>(
                new GeneralInspector(std::move(field_data), std::move(function_data)));
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
        : fields(field_data), functions(function_data)
    {
    }

    void drawFields();
    void drawFunctions();

    std::vector<Field> fields;
    std::vector<std::function<void()>> functions;

};  // class GeneralInspector
}  // namespace ui
}  // namespace seg

#endif