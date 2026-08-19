#include <iostream>
#include <ostream>
#include <vector>
#include <string_view>

const char *lorem_ipsum =
        "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Donec sit amet ante odio. Curabitur finibus congue nisl sed cursus. Curabitur elementum, risus sed facilisis blandit, lorem nulla ultrices neque, id pellentesque tortor erat laoreet nisl. Etiam nec turpis feugiat, porttitor est quis, dapibus lorem. Etiam tristique, nisl bibendum pellentesque mattis, nisi leo tempus quam, consectetur ullamcorper justo lorem ut ligula. Vestibulum non volutpat massa, a finibus purus. Mauris vulputate sed nibh eget malesuada. Morbi eu gravida arcu, facilisis cursus arcu."
        "Mauris vestibulum massa quis leo feugiat molestie. Nulla placerat ipsum sapien, ac consequat elit sodales ut. Fusce in auctor libero. Etiam consequat eu elit quis molestie. Quisque ac iaculis felis. Duis condimentum commodo vulputate. Integer vitae vulputate turpis. Nulla pulvinar quis nulla vel vestibulum. Duis interdum mollis turpis, sed porttitor nisi porta at. Vestibulum porttitor mi purus, nec malesuada risus tristique feugiat. Sed at lobortis enim. ";

constexpr std::size_t MAX_CHUNK_LEN = 64;

int main() {
    std::string_view lorem_ipsum_s = lorem_ipsum;
    std::vector<std::string_view> parts;
    for (int i = 0; i < lorem_ipsum_s.length(); i += MAX_CHUNK_LEN) {
        auto chunk_len = std::min(MAX_CHUNK_LEN, lorem_ipsum_s.length() - i);
        parts.push_back(lorem_ipsum_s.substr(i, chunk_len));
    }
    for (auto part: parts) {
        std::cout << part << "\n";
    }
    return 0;
}