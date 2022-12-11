/*
 * string.hpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#ifndef ITO_CORE_STRING_H_
#define ITO_CORE_STRING_H_

#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <typeindex>

#include "base.hpp"

namespace ito {
namespace str {

/** ---- Stringify function ---------------------------------------------------
 * to_string<...>
 * @brief String represention of all the arguments. Use a variadic template to
 * iterate over each argument and construct the concatenated string recursively.
 * @see https://en.cppreference.com/w/cpp/language/parameter_pack
 *      https://eli.thegreenplace.net/2014/variadic-templates-in-c
 *      https://stackoverflow.com/questions/54744252
 */
template<typename... Args>
ito_inline std::string to_string(Args&&... args);

template<typename T, typename... Args>
ito_inline std::string to_string(T t, Args&&... args)
{
    std::ostringstream ss;
    ss << t << to_string(static_cast<Args&&>(args)...);
    return ss.str();
}

template<>
ito_inline std::string to_string()
{
    return std::string();
}

/** ---- String manipulation functions ----------------------------------------
 * left_trim
 * @brief Trim off left characters from the string (white-space by default).
 *
 * string::find_first_not_of(const string &chars, size_t pos = 0)
 *
 * Find the *first* character in the string which is not in the character
 * sequence over the interval [pos = 0, size()).
 * If no character is found, npos is returned, i.e. greatest possible value for
 * an element of type size_t.
 * If npos is returned, then effectively *every* character in the string is in
 * the character sequence.
 * Calling erase with count = npos means all string elements are removed.
 *
 * string::erase(size_t pos, size_t count = npos)
 *
 * Erase the substring beginning at position pos and spanning count characters
 * (or until end of the string if either size() is too short or if count is
 * string::npos).
 */
ito_inline std::string &left_trim(
    std::string &str,
    const std::string &chars = "\t\n\v\f\r ")
{
    str.erase(0, str.find_first_not_of(chars));
    return str;
}

/**
 * right_trim
 * @brief Trim off right characters from the string(white-space by default).
 *
 * string::find_last_not_of(const string &chars, size_t pos = npos)
 *
 * Find the *last* character in the string which is not in the character
 * sequence over the interval [0, pos = npos). A pos value greater or equal
 * to size() means entire string is searched.
 *
 * If a character is not found, npos is returned, i.e. greatest possible value
 * for an element of type size_t.
 * If npos is returned, then effectively *every* character in the string is in
 * the character sequence.
 * Calling erase with pos = npos + 1 (= 0) means all elements are removed.
 *
 * string::erase(size_t pos, size_t count = npos)
 *
 * Erase the substring beginning at position pos and spanning count characters
 * (or until end of the string if either size() is too short or if count is
 * string::npos).
 */
ito_inline std::string &right_trim(
    std::string &str,
    const std::string &chars = "\t\n\v\f\r ")
{
    str.erase(str.find_last_not_of(chars) + 1);
    return str;
}

/**
 * trim
 * @brief Trim off left and right characters from the string. The default
 * characters are white-space characters as given by the "C" locale:
 *
 * ' '  (0x20)    space(SPC)
 * '\t' (0x09)    horizontal tab(TAB)
 * '\n' (0x0a)    newline(LF)
 * '\v' (0x0b)    vertical tab(VT)
 * '\f' (0x0c)    feed(FF)
 * '\r' (0x0d)    carriage return (CR)
 *
 * The trim functions return a reference to the operated string,
 * thereby allowing these to be chained together.
 */
ito_inline std::string &trim(
    std::string &str, const std::string &chars = "\t\n\v\f\r ")
{
    return left_trim(right_trim(str, chars), chars);
}

/**
 * replace
 * @brief Scan the string and replace any characters given by search by the
 * characters given by replace.
 */
ito_inline std::string &replace(
    std::string &str, const char search, const char replace)
{
    /* Check the trivial case */
    if (search == replace) {
        return str;
    }

    /* Scan the string for search and replace */
    std::size_t tok = 0;
    while ((tok = str.find_first_of(search, tok)) != std::string::npos) {
        str[tok++] = replace;
    }

    return str;
}

/**
 * uncomment
 * @brief Trim off string comments.
 *
 * Scan the string and remove all characters between the comment symbol '#' and
 * the end character.
 * For a single line where the newline character has been replaced by the null
 * terminator, end='\0'. For a full stream buffer with newline characters,
 * end='\n' should be used instead.
 */
ito_inline std::string &uncomment(std::string &str, const char end)
{
    /*
     * Find the first character that matches comment '#' and replace all
     * characters by space until the end.
     */
    std::size_t beg = 0;
    while ((beg = str.find_first_of('#', beg)) != std::string::npos) {
        std::size_t tok = beg++;
        while ((tok < str.size()) && (str[tok] != end)) {
            str[tok++] = '\0';
        }
    }

    return str;
}

/** ---- C-string manipulation functions --------------------------------------
 * left_trim
 * @brief Trim off left spaces from the string.
 * Find the first non space character or stop at the end.
 * Ensure the value isspace takes is of type unsigned char.
 */
ito_inline char *left_trim(char *str)
{
    /* Scan from beginning and find the first non space character */
    char *beg = str;
    while ((*beg != '\0') && std::isspace((unsigned char) *beg)) {
        beg++;
    }

    /* Shift chars in place if needed */
    char *dst = str;
    if (beg != dst) {
        while ((*dst++ = *beg++));    /* stop at first null terminator */
    }

    return str;
}

/**
 * right_trim
 * @brief Trim off right spaces from the string.
 *
 * Start from the last character in the string and replace any space character
 * by a null terminator. Stop at beginning or at the first non space character.
 * Ensure the value isspace takes is of type unsigned char.
 */
ito_inline char *right_trim(char *str)
{
    /* Scan from end to beg and replace space by null terminator */
    char *beg = str;
    char *end = str + std::strlen(str) - 1;
    while ((end >= beg) && std::isspace((unsigned char) *end)) {
        *end-- = '\0';
    }

    return str;
}

/**
 * trim
 * @brief Trim off left and right spaces from the string.
 */
ito_inline char *trim(char *str)
{
    return left_trim(right_trim(str));
}

/**
 * replace
 * @brief Scan the string and replace any characters given by search by the
 * characters given by replace.
 */
ito_inline char *replace(char *str, const char search, const char replace)
{
    /* Check the trivial case */
    if (search == replace) {
        return str;
    }

    /* Scan the string for search and replace */
    char *tok = str;
    while ((tok = std::strchr(tok, search)) != nullptr) {
        *tok++ = replace;
    }

    return str;
}

/**
 * uncomment
 * @brief Trim off string comments.
 * Scan the string and remove all characters between the comment symbol '#' and
 * the end character.
 *
 * For a single line where the newline character has been replaced by the null
 * terminator, end='\0'.
 * For a full stream buffer with newline characters, end='\n' should be used
 * instead.
 */
ito_inline char *uncomment(char *str, const char end)
{
    /*
     * Find the first character that matches comment '#' and replace all
     * characters by space until the end.
     */
    char *beg = str;
    while ((beg = std::strchr(beg, '#')) != nullptr) {
        char *tok = beg++;
        while ((*tok != '\0') && (*tok != end)) {
            *tok++ = '\0';
        }
    }

    return str;
}

/**
 * numcpy
 * @brief Copy the first num-1 characters of source word to destination str.
 *
 * If the source is shorter than num-1 characters, the destination is padded
 * with zeros until a total num characters have been written to it.
 * Unlike strncpy, a null character is implicitly appended at the end of
 * destination if source is shorter than num.
 */
ito_inline char *numcpy(char *str, const char *word, long num)
{
    /*
     * Copy the first num-1 characters of the source string to the destination.
     */
    long len = std::strlen(word);
    char *dst = str;
    while ((dst-str < len) && (dst-str < num-1)) {
        *dst++ = *word++;
    }

    /*
     * Pad the remaining destination characters with zeros until num characters
     * have been written.
     */
    while (dst-str < num) {
        *dst++ = '\0';
    }

    /* Return destination string */
    return str;
}

/** ---- String token functions -----------------------------------------------
 * next_token
 * @brief Retrive the next token found in a string.
 *
 * Unlike strtok, this function does not modify the input string and copies
 * instead the next token found into the word argument.
 *
 * If word = nullptr, the copy is not performed.
 *
 * If the word length is shorter than the token length, only the first num-1
 * characters are copied.
 *
 * A null terminator is implictly added at the end.
 *
 * The implicit delimiter separating the string tokens is white-space
 * character as given by the "C" locale:
 *  ' '  (0x20)    space(SPC)
 *  '\t' (0x09)    horizontal tab(TAB)
 *  '\n' (0x0a)    newline(LF)
 *  '\v' (0x0b)    vertical tab(VT)
 *  '\f' (0x0c)    feed(FF)
 *  '\r' (0x0d)    carriage return (CR)
 *
 * The function returns a pointer to the next token. A null pointer is returned
 * if there are no tokens left in the substring or an EOF is reached.
 */
ito_inline char *next_token(const char *str)
{
    /* Token of the first non space character */
    char *beg = (char *) str;
    while ((*beg != '\0') && std::isspace((unsigned char) *beg)) {
        beg++;
    }
    if (*beg == '\0') {
        return nullptr;     /* reached end-of-string */
    }

    /* Token of the next space character */
    char *end = beg;
    while ((*end != '\0') && !std::isspace((unsigned char) *end)) {
        end++;
    }

    return end;
}

/**
 * count_tokens
 * @brief Count the number of tokens delimited by white space in a string.
 */
ito_inline size_t count_tokens(char *str)
{
    size_t count = 0;
    char *cur = str;
    while ((cur = next_token(cur)) != nullptr) {
        ++count;
    }
    return count;
}

/**
 * split_tokens
 * @brief Split the string into tokens delimited by white space.
 *
 * Scan first the string to count the total number of tokens and create a new
 * token structure with a maximum word length given by the argument length.
 * Scan the string again and store all the tokens found into the newly created
 * object.
 */
ito_inline char **split_tokens(char *str, size_t *n_tokens)
{
    /* Do not split a null string */
    if (str == nullptr) {
        return nullptr;
    }

    /* Scan the string and count number of token words */
    *n_tokens = count_tokens(str);
    if (*n_tokens == 0) {
        return nullptr;
    }

    /* Create an array of strings to store all the tokens */
    char **tokens = (char **) calloc(*n_tokens, sizeof(char *));
    ito_assert(tokens != nullptr, "failed to alloc tokens");

    /* Rescan the string and store the token words */
    size_t ix = 0;
    char *end = str;
    char *beg = end;
    while ((end = next_token(beg)) != nullptr) {
        /* Create a new word with num + 1 chars including null terminator */
        int num = (end - beg) / sizeof(char);
        char *word = (char *) calloc(num + 1, sizeof(char));
        ito_assert(word != nullptr, "failed to alloc word");

        /* Copy the token characters */
        char *src = beg;
        char *dst = word;
        while ((dst-word < num) && (src < end)) {
            *dst++ = *src++;
        }
        *dst = '\0';

        /* Store the token word */
        tokens[ix++] = word;

        /* Next token */
        beg = end;
    }

    return tokens;
}

/**
 * split_tokens
 * @brief Split the string into tokens delimited by white space.
 *
 * Scan the string first and count the total number of tokens.
 * If the string has no tokens, do not clear the current token set.
 * Otherwise, rescan the string and store each token sequentially.
 */
ito_inline std::vector<std::string> split_tokens(char *str)
{
    std::vector<std::string> tokens;

    /* Do not split a null string */
    if (str == nullptr) {
        return tokens;
    }

    /* Scan the string and store the token words */
    char *end = str;
    char *beg = end;
    while ((end = next_token(beg)) != nullptr) {
        tokens.push_back(std::string(beg, end - beg));
        beg = end;
    }
    return tokens;
}

/** ---- String cast conversion functions -------------------------------------
 * @brief Parse the string interpreting its content as a number. The cast
 * functions handle the string conversion to the fundamental data types:
 *
 *  floating point types:                   long double, double, float
 *  integer types, signed and unsigned:     long long, long, int, short
 *
 * The cast function names are defined using the OpenGL suffix convention
 * {ld, d, f, s,, i, l, ll, us, ui, ul, ull}.
 *
 * These functions handle success or failure reported by the corresponding
 * strto functions - endptr and errno.
 *
 * If the endptr is not nullptr, a pointer to the character after the last
 * used in the conversion is stored in the location referenced by endptr.
 *
 * If the correct value would cause overflow, plus or minus HUGE_VAL, HUGE_VALF,
 * or HUGE_VALL is returned - according to the sign and type of the return
 * value; ERANGE is stored in errno. If the correct value would cause underflow,
 * zero is returned and ERANGE is stored in errno.
 */

/**
 * castld
 */
ito_inline long double castld(const char *str)
{
    errno = 0;
    char *endptr;
    long double x = std::strtold(str, &endptr);
    ito_assert(errno != ERANGE &&
               *endptr == '\0' &&
               str != endptr,
               "invalid number");
    return x;
}

/**
 * castd
 */
ito_inline double castd(const char *str)
{
    errno = 0;
    char *endptr;
    double x = std::strtod(str, &endptr);
    ito_assert(errno != ERANGE &&
               *endptr == '\0' &&
               str != endptr,
               "invalid number");
   return x;
}

/**
 * castf
 */
ito_inline float castf(const char *str)
{
    errno = 0;
    char *endptr;
    float x = std::strtof(str, &endptr);
    ito_assert(errno != ERANGE &&
               *endptr == '\0' &&
               str != endptr,
               "invalid number");
    return x;
}

/**
 * castll
 */
ito_inline long long castll(const char *str)
{
    errno = 0;
    char *endptr;
    long long x = std::strtoll(str, &endptr, 0);
    ito_assert(errno != ERANGE &&
               *endptr == '\0' &&
               str != endptr,
               "invalid number");
    return x;
}

/**
 * castl
 * @brief Parse the string as a long integer.
 */
ito_inline long castl(const char *str)
{
    errno = 0;
    char *endptr;
    long x = std::strtol(str, &endptr, 0);
    ito_assert(errno != ERANGE &&
               *endptr == '\0' &&
               str != endptr,
               "invalid number");
    return x;
}

/**
 * casti
 * @brief Parse the string as an integer.
 */
ito_inline int casti(const char *str)
{
    errno = 0;
    char *endptr;
    long x = std::strtol(str, &endptr, 0);
    ito_assert(errno != ERANGE &&
               *endptr == '\0' &&
               str != endptr,
               "invalid number");

#if (LONG_MIN < INT_MIN || LONG_MAX > INT_MAX)
    /* Ensure 'long' value is within 'int' range */
    if (x < INT_MIN || x > INT_MAX) {
        errno = ERANGE;
        ito_throw("range error");
    }
#endif

    return (int) x;
}

/**
 * casts
 * @brief Parse the string as a short integer.
 */
ito_inline short casts(const char *str)
{
    errno = 0;
    char *endptr;
    long x = std::strtol(str, &endptr, 0);
    ito_assert(errno != ERANGE &&
               *endptr == '\0' &&
               str != endptr,
               "invalid number");

#if (LONG_MIN < SHRT_MIN || LONG_MAX > SHRT_MAX)
    /* Ensure 'long' value is within 'short' range */
    if (x < SHRT_MIN || x > SHRT_MAX) {
        errno = ERANGE;
        ito_throw("range error");
    }
#endif

    return (short) x;
}

/**
 * castull
 * @brief Parse the string as a unsigned long long integer.
 */
ito_inline unsigned long long castull(const char *str)
{
    errno = 0;
    char *endptr;
    unsigned long long x = std::strtoull(str, &endptr, 0);
    ito_assert(errno != ERANGE &&
               *endptr == '\0' &&
               str != endptr,
               "invalid number");
    return x;
}

/**
 * castul
 * @brief Parse the string as a unsigned long integer.
 */
ito_inline unsigned long castul(const char *str)
{
    errno = 0;
    char *endptr;
    unsigned long x = std::strtoul(str, &endptr, 0);
    ito_assert(errno != ERANGE &&
               *endptr == '\0' &&
               str != endptr,
               "invalid number");
    return x;
}

/**
 * castui
 * @brief Parse the string as a usigned integer.
 */
ito_inline unsigned int castui(const char *str)
{
    errno = 0;
    char *endptr;
    unsigned long x = std::strtoul(str, &endptr, 0);
    ito_assert(errno != ERANGE &&
               *endptr == '\0' &&
               str != endptr,
               "invalid number");

#if (ULONG_MAX > UINT_MAX)
    /* Ensure 'unsigned long' value is within 'unsigned int' range */
    if (x > UINT_MAX) {
        errno = ERANGE;
        ito_throw("range error");
    }
#endif

    return (unsigned int) x;
}

/**
 * castus
 * @brief Parse the string as a unsigned sort integer.
 */
ito_inline unsigned short castus(const char *str)
{
    errno = 0;
    char *endptr;
    unsigned long x = std::strtoul(str, &endptr, 0);
    ito_assert(errno != ERANGE &&
               *endptr == '\0' &&
               str != endptr,
               "invalid number");

#if (ULONG_MAX > USHRT_MAX)
    /* Ensure 'unsigned long' value is within 'unsigned int' range */
    if (x > USHRT_MAX) {
        errno = ERANGE;
        ito_throw("range error");
    }
#endif

    return (unsigned short) x;
}

/**
 * @brief Templated string cast conversion function.
 * Interface handling string casting into one of the fundamental types.
 * If none found, it tries to create a string stream and extract the
 * formatted result. If no conversion can be performed, 0 is returned.
 * @see https://en.cppreference.com/w/cpp/language/types
 *      https://en.cppreference.com/w/cpp/string/byte/strto[ld,d,f]
 *      https://en.cppreference.com/w/cpp/string/byte/strto[ll,l,ull,ul]
 */
template<typename T>
ito_inline T cast(const char *str)
{
    ito_assert(str != nullptr, "invalid string");

    /*
     * Check fundamental types.
     */
    if (std::is_same<T, long double>::value)        { return castld(str); }
    if (std::is_same<T, double>::value)             { return castd(str); }
    if (std::is_same<T, float>::value)              { return castf(str); }
    if (std::is_same<T, long long>::value)          { return castll(str); }
    if (std::is_same<T, long>::value)               { return castl(str); }
    if (std::is_same<T, int>::value)                { return casti(str); }
    if (std::is_same<T, short>::value)              { return casts(str); }
    if (std::is_same<T, char>::value)               { return casts(str); }
    if (std::is_same<T, unsigned long long>::value) { return castull(str); }
    if (std::is_same<T, unsigned long>::value)      { return castul(str); }
    if (std::is_same<T, unsigned int>::value)       { return castui(str); }
    if (std::is_same<T, unsigned short>::value)     { return castus(str); }
    if (std::is_same<T, unsigned char>::value)      { return castus(str); }

    /*
     * Check fixed width integer types.
     */
    if (std::is_same<T, int64_t>::value)  { return castl(str); }
    if (std::is_same<T, int32_t>::value)  { return castd(str); }
    if (std::is_same<T, int16_t>::value)  { return casts(str); }
    if (std::is_same<T, int8_t>::value)   { return casts(str); }
    if (std::is_same<T, uint64_t>::value) { return castul(str); }
    if (std::is_same<T, uint32_t>::value) { return castui(str); }
    if (std::is_same<T, uint16_t>::value) { return castus(str); }
    if (std::is_same<T, uint8_t>::value)  { return castus(str); }

    /*
     * Otherwise, create a string stream and extract the formatted result.
     */
    std::istringstream is(str);
    T result;
    if (is >> result) {
        return result;
    }

    /*
     * No valid conversion.
     */
    return static_cast<T>(0);
}

/**
 * @brief Templated string cast conversion function.
 * Overloaded function using std::string signature.
 */
template<typename T>
ito_inline T cast(const std::string &str)
{
    return cast<T>(str.c_str());
}

/** ---- String format generation functions -----------------------------------
 * @brief Return a format string with a snprintf style.
 * @see https://en.cppreference.com/w/cpp/io/c/fprintf
 *      https://stackoverflow.com/questions/2342162
 */
template<typename... Args>
ito_inline std::string format(const std::string& fmt, const Args&... args)
{
    /*
     * Calling std::snprintf with zero size and nullptr for buffer is useful to
     * determine the necessary buffer size to contain the output.
     * std::snprintf returns the number of characters written if successful or
     * a negative value if an error occurred.
     */
    int size = std::snprintf(nullptr, 0, fmt.c_str(), args...);
    ito_assert(size >= 0, "error generating formatted string");

    /*
     * Create a buffer with size+1 to include all the format characters plus
     * the null terminator '\0'.
     */
    std::vector<char> buffer(size + 1, '\0');

    /* Write the formatted string to the newly created buffer. */
    int ret = std::snprintf(buffer.data(), size + 1, fmt.c_str(), args...);
    ito_assert(ret == size, "error generating formatted string");

    /* Return the formatted string without the null terminator '\0'. */
    return std::string(buffer.data(), size);
}

} /* str */
} /* ito */

#endif /* ITO_CORE_STRING_H_ */
