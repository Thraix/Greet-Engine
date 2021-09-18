#include "ImageUtils.h"

#include <logging/Log.h>
#include <graphics/textures/ImageFactory.h>
#include <utils/ErrorHandle.h>

#include <FreeImage.h>
#include <fstream>

namespace Greet {

  void ImageUtils::PrintImage(const ImageData& imageData)
  {
    for (uint32_t y = 0;y < imageData.height; y++)
    {
      for (uint32_t x = 0;x < imageData.width; x++)
      {
        Log::Info("pixel(%s, %s) (R=%s, G=%s, B=%s)", x, y, (int)imageData.at(x, y, IMAGE_DATA_RED), (int)imageData.at(x, y, IMAGE_DATA_GREEN), (int)imageData.at(x, y, IMAGE_DATA_BLUE));
      }
    }
  }

  ImageData ImageUtils::FlipImage(const ImageData& imageData)
  {
    ImageData result(imageData.width, imageData.height);
    for (uint32_t y = 0;y < imageData.height;y++)
    {
      for (uint32_t x = 0;x < imageData.width;x++)
      {
        result.at(imageData.width - x - 1, imageData.height - y - 1) = imageData.at(x, y);
      }
    }
    return result;
  }

  ImageDataResult ImageUtils::LoadImage(const std::string& filepath)
  {
    FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;

    FIBITMAP *dib = nullptr;

    fif = FreeImage_GetFileType(filepath.c_str(), 0);

    if (fif == FIF_UNKNOWN)
      fif = FreeImage_GetFIFFromFilename(filepath.c_str());

    if (fif == FIF_UNKNOWN)
    {
      Log::Error("FreeImage file format is not supported or file not exist: ", filepath);
      ErrorHandle::SetErrorCode(GREET_ERROR_IMAGE_FORMAT);
      return {false, ImageFactory::GetBadFormatImage()};
    }

    if (FreeImage_FIFSupportsReading(fif))
      dib = FreeImage_Load(fif, filepath.c_str());
    if (!dib)
    {
      Log::Error("FreeImage file Cannot be read: ", filepath);
      ErrorHandle::SetErrorCode(GREET_ERROR_IMAGE_READ);
      return {false, ImageFactory::GetCantReadImage()};
    }


    uint8_t* fiBits = FreeImage_GetBits(dib);

    uint8_t bpp = FreeImage_GetBPP(dib);
    bpp >>= 3;
    if (bpp != 3 && bpp != 4)
    {
      Log::Error("Bits per pixel is not valid (24 or 32): %s %s bpp", filepath, bpp * 8);
      ErrorHandle::SetErrorCode(GREET_ERROR_IMAGE_BPP);
      FreeImage_Unload(dib);
      return {false, ImageFactory::GetBadBPPImage()};
    }

    ImageData result{FreeImage_GetWidth(dib), FreeImage_GetHeight(dib)};
    uint32_t bitsI = 0;
    for(uint32_t y = 0; y < result.height; y++)
    {
      for(uint32_t x = 0; x < result.width; x++)
      {
        // Incase the order of FreeImage is not RGB (its probably BGRA)
        result.at(x, y, IMAGE_DATA_RED) = fiBits[bitsI + FI_RGBA_RED];
        result.at(x, y, IMAGE_DATA_GREEN) = fiBits[bitsI + FI_RGBA_GREEN];
        result.at(x, y, IMAGE_DATA_BLUE) = fiBits[bitsI + FI_RGBA_BLUE];
        if(bpp == 4)
          result.at(x, y, IMAGE_DATA_ALPHA) = fiBits[bitsI + FI_RGBA_ALPHA];
        else
          result.at(x, y, IMAGE_DATA_ALPHA) = 0xff;
        bitsI += bpp;
      }
    }
    FreeImage_Unload(dib);
    return {true, result};
  }

  ImageData ImageUtils::CreateHeightmapImage(const std::vector<float>& heightMap, uint32_t width, uint32_t height)
  {
    ImageData result(width, height);
    for(uint32_t y = 0; y < height; y++)
    {
      for(uint32_t x = 0; x < width; x++)
      {
        uint8_t color = heightMap[x + y * width] * 255;
        result.at(x, y, IMAGE_DATA_RED) = color;
        result.at(x, y, IMAGE_DATA_GREEN) = color;
        result.at(x, y, IMAGE_DATA_BLUE) = color;
        result.at(x, y, IMAGE_DATA_ALPHA) = 0xff;
      }
    }
    return result;
  }

  ImageData ImageUtils::CropImage(const ImageData& imageData, uint32_t cx, uint32_t cy, uint32_t cwidth, uint32_t cheight)
  {
    if (cx >= imageData.width || cy >= imageData.height || cx + cwidth > imageData.width || cy + cheight > imageData.height)
    {
      Log::Error("Invalid bounds when cropping image: %s, %s, %s, %s", cx, cy, cwidth, cheight);
      ErrorHandle::SetErrorCode(GREET_ERROR_IMAGE_CROP);
      return ImageFactory::GetCropErrorImage();
    }
    cy = imageData.height - cheight - cy;
    ImageData result(cwidth, cheight);
    size_t offset = (cx + cy * imageData.width) * 4;
    size_t resultOffset = 0;
    for (uint32_t y = 0;y < cheight;y++)
    {
      memcpy(result.data.get() + resultOffset, imageData.data.get() + offset, cwidth * 4);
      offset += imageData.width * 4;
      resultOffset += cwidth * 4;
    }
    return result;
  }

  void ImageUtils::SaveImageBytes(const std::string& filepath, const std::string& output)
  {
    auto res = LoadImage(filepath);
    if(res.first)
    {
      std::ofstream fout;
      fout.open(output, std::ios_base::binary | std::ios_base::out);
      fout.write((char*) res.second.data.get(), res.second.width * res.second.height * 4);

      fout.close();
    }
  }
}
