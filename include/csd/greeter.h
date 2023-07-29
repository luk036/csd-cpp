#pragma once

#include <iosfwd> // for string
#include <string> // for basic_string

namespace csd {

/**  Language codes to be used with the Csd class */
enum class LanguageCode { EN, DE, ES, FR };

/**
 * @brief A class for saying hello in multiple languages
 */
class Csd {
    std::string name;

  public:
    /**
     * @brief Creates a new csd
     * @param name the name to greet
     */
    Csd(std::string name);

    /**
     * @brief Creates a localized string containing the greeting
     * @param lang the language to greet in
     * @return a string containing the greeting
     */
    std::string greet(LanguageCode lang = LanguageCode::EN) const;
};

} // namespace csd
