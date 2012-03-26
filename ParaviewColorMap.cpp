#include "itkCovariantVector.h"
#include "itkCustomColormapFunction.h"
#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkScalarToRGBColormapImageFilter.h"

typedef itk::Image<float, 2>  ImageType;

typedef itk::RGBPixel<unsigned char> RGBPixelType;
typedef itk::Image<RGBPixelType, 2>  RGBImageType;

// This application converts a single channel float image to a psuedo colored RGB png based on the "cool to warm" colormap that Paraview uses as its default colormap.
int main(int argc, char *argv[])
{
  // Verify arguments
  if(argc < 3)
    {
    std::cerr << "Required: inputFilename outputFilename" << std::endl;
    return EXIT_FAILURE;
    }

  // Parse arguments
  std::string inputFilename = argv[1];
  std::string outputFilename = argv[2];

  // Output arguments
  std::cout << "inputFilename " << inputFilename << std::endl;
  std::cout << "outputFilename " << outputFilename << std::endl;

  // Read file
  typedef itk::ImageFileReader<ImageType> ReaderType;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(inputFilename);
  reader->Update();

  typedef itk::Function::CustomColormapFunction<ImageType::PixelType, RGBImageType::PixelType> ColormapType;
  ColormapType::ChannelType redChannel;
  ColormapType::ChannelType greenChannel;
  ColormapType::ChannelType blueChannel;

  redChannel.push_back(static_cast<ColormapType::RealType>(0.231373));
  greenChannel.push_back(static_cast<ColormapType::RealType>(0.298039));
  blueChannel.push_back(static_cast<ColormapType::RealType>(0.752941));

  redChannel.push_back(static_cast<ColormapType::RealType>(1));
  greenChannel.push_back(static_cast<ColormapType::RealType>(1));
  blueChannel.push_back(static_cast<ColormapType::RealType>(1));

  redChannel.push_back(static_cast<ColormapType::RealType>(0.705882));
  greenChannel.push_back(static_cast<ColormapType::RealType>(0.0156863));
  blueChannel.push_back(static_cast<ColormapType::RealType>(0.14902));

  ColormapType::Pointer colormap = ColormapType::New();
  colormap->SetRedChannel(redChannel);
  colormap->SetGreenChannel(greenChannel);
  colormap->SetBlueChannel(blueChannel);

  // Setup conversion
  typedef itk::ScalarToRGBColormapImageFilter<ImageType, RGBImageType> RGBFilterType;
  RGBFilterType::Pointer rgbfilter = RGBFilterType::New();
  rgbfilter->SetInput(reader->GetOutput());

  rgbfilter->SetColormap(colormap);

  // Write the result
  typedef  itk::ImageFileWriter<RGBImageType> WriterType;
  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName(outputFilename);
  writer->SetInput(rgbfilter->GetOutput());
  writer->Update();

  return EXIT_SUCCESS;
}
