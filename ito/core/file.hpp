/*
 * file.hpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#ifndef ITO_CORE_FILE_H_
#define ITO_CORE_FILE_H_

#include <vector>
#include <limits>

namespace ito {

/** ---- File type ------------------------------------------------------------
 * @brief Plain data structure maintaining a C-style std::FILE stream.
 */
struct FileDeleter {
    bool is_owner = false;
    void operator()(std::FILE *file) {
        if (is_owner && file != nullptr) {
            std::fclose(file);
        }
    }
};

using File = std::unique_ptr<std::FILE, FileDeleter>;

ito_inline File make_file(std::FILE *file)
{
    FileDeleter file_deleter{false};
    return File(file, file_deleter);
}

ito_inline File make_file(const char *filename, const char *filemode)
{
    FileDeleter file_deleter{true};
    return File(std::fopen(filename, filemode), file_deleter);
}

ito_inline File make_file(
    const std::string &filename, const std::string &filemode)
{
    return make_file(filename.c_str(), filemode.c_str());
}

namespace file {

/** ---- File query -----------------------------------------------------------
 * @brief Return the length of the binary stream in bytes from the beginning of
 * the file. Return -1L if failure occurs.
 */
ito_inline int64_t length(File &file)
{
    std::fseek(file.get(), 0, SEEK_END);   /* go to end of file */
    int64_t len = std::ftell(file.get());  /* get length in bytes */
    std::fseek(file.get(), 0, SEEK_SET);   /* go back to begining of file */
    return len;
}

/** ---- File input -----------------------------------------------------------
 * @brief Templated scanf function.
 */
template<typename... Args>
ito_inline int64_t scanf(File &file, const char *format, Args&&... args)
{
    return std::fscanf(file.get(), format, static_cast<Args&&>(args)...);
}

/**
 * @brief Read one block of size bytes as if calling fgetc size times.
 * Return the number of characters successfuly read (or less than 1 if
 * an error or eof condition occurs).
 */
ito_inline int64_t read(File &file, void *ptr, size_t size)
{
    return std::fread(ptr, size, 1, file.get());
}

/**
 * @brief Read a line from an input stream and strip the delim character.
 * Stop when either:
 *  - maximum characters are read (default max(size_t)) or
 *  - error reading character (fgetc returns EOF) or
 *  - delim character is read (default newline '\n')
 * whichever comes first.
 * Return the number of characters read on success. Return EOF on failure.
 *
 * @note Handle null '\0' and newline '\n' characters:
 *  - string automatically appends a null character '\0'
 *
 *  - strlen has undefined behaviour for an empty buffer beginning with '\0',
 *      if (*(str + std::strlen(str) - 1) == '\n')
 *          *(str + std::strlen(str) - 1) = '\0';
 *
 *  - strcspn counts the number of characters until it finds a '\n' character
 *  or a '\r' character. If none found, strcspn() stops at the null terminator
 *  '\0' and returns 0. The operation is redudant but safe by simply replacing
 *  '\0' with the same character '\0':
 *      str[std::strcspn(str, "\n")] = '\0';
 *
 * @see https://stackoverflow.com/questions/8558907
 */
ito_inline bool readline(
    File &file,
    std::string &line,
    const size_t count = std::numeric_limits<size_t>::max(),
    const char delim = '\n')
{
    line.clear();
    while (line.size() < count) {
        int c = std::fgetc(file.get());
        if (c == EOF) {     /* note: use int, not char, to handle EOF */
            return false;
        }

        if (c == delim) {   /* read newline or end character */
            break;
        }

        line.push_back(c);
    }
    return true;
}

/**
 * @brief Read all lines from an input stream into a vector. Each line reads
 * characters from the stream until a delim character is read (default '\n').
 */
ito_inline bool readlines(
    File &file,
    std::vector<std::string> &lines,
    const size_t count = std::numeric_limits<size_t>::max(),
    const char delim = '\n')
{
    std::string buffer;
    while (readline(file, buffer, count, delim)) {
        if (!buffer.empty()) {
            lines.push_back(buffer);
        }
    }
    return true;
}

/** ---- File output ----------------------------------------------------------
 * @brief Templated printf function.
 */
template<typename... Args>
ito_inline int64_t printf(File &file, const char *format, Args&&... args)
{
    return std::fprintf(file.get(), format, static_cast<Args&&>(args)...);
}

/**
 * @brief Write one block of size bytes as if calling fputc size times.
 * Return the number of characters successfuly written (or less than 1 if
 * an error or eof condition occurs).
 */
ito_inline int64_t write(File &file, void *ptr, size_t size)
{
    return std::fwrite(ptr, size, 1, file.get());
}

/**
 * @brief Write the line string to the output stream. Return EOF on failure.
 */
ito_inline bool writeline(
    File &file,
    const std::string &line,
    const std::string &sep = "\n")
{
    bool success = (std::fputs(line.c_str(), file.get()) != EOF);
    if (success && !sep.empty()) {
        success = (std::fputs(sep.c_str(), file.get()) != EOF);
    }
    return success;
}

/**
 * @brief Write lines to the stream using writeline. Return EOF on failure.
 */
ito_inline bool writelines(
    File &file,
    const std::vector<std::string> &lines,
    const std::string &sep = "\n")
{
    std::vector<std::string>::const_iterator it = lines.begin();
    while (it != lines.end()) {
        if (!writeline(file, *it, sep)) {
            return false;
        }
        ++it;
    }
    return true;
}

} /* file */
} /* ito */

#endif /* ITO_CORE_FILE_H_ */
