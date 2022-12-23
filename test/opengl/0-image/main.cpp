/*
 * main.cpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#include "ito/opengl.hpp"
using namespace ito;

/** ---------------------------------------------------------------------------
 * @brief Constants and globals.
 */
static const int kWidth = 800;
static const int kHeight = 800;
static const std::string kWritePrefix = {"/tmp/"};
static const std::string kReadPrefix = {"../common/"};
static const std::vector<std::string> kImageFilenames = {
    "baboon_512.png",
    "color-wheel-80x80-blue.png",
    "color-wheel-80x80-bluea.png",
    "color-wheel-80x80-red.png",
    "color-wheel-80x80-reda.png",
    "color-wheel-80x80-rgb.png",
    "color-wheel-80x80-rgba.png",
    "color-wheel-rgb.png",
    "color-wheel-rgba.png",
    "equirectangular.png",
    "fruits_512.png",
    "monarch_512.png",
    "pool_512.png"};

/** ---------------------------------------------------------------------------
 * main test client
 */
int main(int argc, char const *argv[])
{
    /* ---- Test Image::Create ------------------------------------------------
     */
    {
        gl::Image img8  = gl::Image::Create(kWidth, kHeight, 8);
        std::cout << gl::Image::InfoStr(img8, "bpp = 8") << "\n";

        gl::Image img16 = gl::Image::Create(kWidth, kHeight, 16);
        std::cout << gl::Image::InfoStr(img16, "bpp = 16") << "\n";

        gl::Image img24 = gl::Image::Create(kWidth, kHeight, 24);
        std::cout << gl::Image::InfoStr(img24, "bpp = 24") << "\n";

        gl::Image img32 = gl::Image::Create(kWidth, kHeight, 32);
        std::cout << gl::Image::InfoStr(img32, "bpp = 32") << "\n";
    }

    /* ---- Test red images ---------------------------------------------------
     */
    {
        std::string filename("color-wheel-80x80-red.png");
        std::string out_png("out.color-wheel-80x80-red.png");
        std::string out_ppma("out.color-wheel-80x80-red_p3.ppm");
        std::string out_ppmb("out.color-wheel-80x80-red_p6.ppm");

        gl::Image image = gl::Image::Load(kReadPrefix + filename);
        std::cout << gl::Image::InfoStr(image, "color-wheel-80x80-red") << "\n";
        gl::Image::SavePng(image, kWritePrefix + out_png);
        gl::Image::SavePpma(image, kWritePrefix + out_ppma);
        gl::Image::SavePpmb(image, kWritePrefix + out_ppmb);
    }

    {
        std::string filename("color-wheel-80x80-reda.png");
        std::string out_png("out.color-wheel-80x80-reda-reda.png");
        std::string out_ppma("out.color-wheel-80x80-reda-reda_p3.ppm");
        std::string out_ppmb("out.color-wheel-80x80-reda-reda_p6.ppm");

        gl::Image image = gl::Image::Load(kReadPrefix + filename);
        std::cout << gl::Image::InfoStr(image, "color-wheel-80x80-reda") << "\n";
        gl::Image::SavePng(image, kWritePrefix + out_png);
        gl::Image::SavePpma(image, kWritePrefix + out_ppma);
        gl::Image::SavePpmb(image, kWritePrefix + out_ppmb);
    }


    /* ---- Test blue images ---------------------------------------------------
     */
    {
        std::string filename("color-wheel-80x80-blue.png");
        std::string out_png("out.color-wheel-80x80-blue.png");
        std::string out_ppma("out.color-wheel-80x80-blue_p3.ppm");
        std::string out_ppmb("out.color-wheel-80x80-blue_p6.ppm");

        gl::Image image = gl::Image::Load(kReadPrefix + filename);
        std::cout << gl::Image::InfoStr(image, "color-wheel-80x80-blue") << "\n";
        gl::Image::SavePng(image, kWritePrefix + out_png);
        gl::Image::SavePpma(image, kWritePrefix + out_ppma);
        gl::Image::SavePpmb(image, kWritePrefix + out_ppmb);
    }

    {
        std::string filename("color-wheel-80x80-bluea.png");
        std::string out_png("out.color-wheel-80x80-bluea-bluea.png");
        std::string out_ppma("out.color-wheel-80x80-bluea-bluea_p3.ppm");
        std::string out_ppmb("out.color-wheel-80x80-bluea-bluea_p6.ppm");

        gl::Image image = gl::Image::Load(kReadPrefix + filename);
        std::cout << gl::Image::InfoStr(image, "color-wheel-80x80-bluea") << "\n";
        gl::Image::SavePng(image, kWritePrefix + out_png);
        gl::Image::SavePpma(image, kWritePrefix + out_ppma);
        gl::Image::SavePpmb(image, kWritePrefix + out_ppmb);
    }


    /* ---- Test image load ---------------------------------------------------
     */
    {
        for (auto &filename : kImageFilenames) {
            std::string out_png = "out." + filename + ".png";
            std::string out_ppma = "out." + filename + "_p3.ppm";
            std::string out_ppmb = "out." + filename + "_p6.ppm";

            gl::Image image = gl::Image::Load(kReadPrefix + filename);

            std::cout << gl::Image::InfoStr(image, filename.c_str()) << "\n";
            gl::Image::SavePng(image, kWritePrefix + out_png);
            gl::Image::SavePpma(image, kWritePrefix + out_ppma);
            gl::Image::SavePpmb(image, kWritePrefix + out_ppmb);
        }
    }

    exit(EXIT_SUCCESS);
}
