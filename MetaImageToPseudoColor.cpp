#include "itkCovariantVector.h"
#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkScalarToRGBColormapImageFilter.h"

typedef itk::Image<float, 2>  ImageType;

typedef itk::RGBPixel<unsigned char>    RGBPixelType;
typedef itk::Image<RGBPixelType, 2>  RGBImageType;

// This application converts a single channel float image to a psuedo colored RGB png.
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

  typedef itk::ScalarToRGBColormapImageFilter<ImageType, RGBImageType> RGBFilterType;
  RGBFilterType::Pointer rgbfilter = RGBFilterType::New();
  rgbfilter->SetInput(reader->GetOutput());
  //rgbfilter->SetColormap( RGBFilterType::Hot );
  rgbfilter->SetColormap( RGBFilterType::Jet );
  rgbfilter->Update();

  // Write the result
  typedef  itk::ImageFileWriter<RGBImageType> WriterType;
  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName(outputFilename);
  writer->SetInput(rgbfilter->GetOutput());
  writer->Update();

  return EXIT_SUCCESS;
}
