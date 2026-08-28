#include <string_view>
#include "word_count.h"

#include <locale>

WordCount transform_to_word_count(const std::string_view &part) {
    if (part.empty()) {
        return WordCount::default_value();
    }
    char first = *part.begin();
    char last = *part.rbegin();
    unsigned long long count = 0;
    bool inside_a_word = false;
    for (const char it : part) {
                if (std::isalnum(it) && !inside_a_word ) {
                    count++;
                    inside_a_word = true;
                } else if (!std::isalnum(it)) {
                    inside_a_word = false;
                }
    }
    return WordCount{!!std::isalnum(first), count, !!std::isalnum(last)};
}
