#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkImageAdaptor.h"
#include "itkRGBToHSVColorSpacePixelAccessor.h"

int main( int argc, char * argv[])
{
  if(argc < 3)
    {
    std::cerr << "Usage: " << argv[0];
    std::cerr << " input output";
    std::cerr << std::endl;
    return EXIT_FAILURE;
    }

   // Set input/output variables.
  std::string imageFileName = argv[1];
  std::string outputFileName = argv[2];
  
  std::cout << "Input: " << imageFileName << std::endl;
  std::cout << "Output: " << outputFileName << std::endl;
  
  // Typedefs
  typedef itk::RGBPixel<unsigned char> RGBPixelType;
  typedef itk::Image<RGBPixelType, 2> RGBImageType;
  typedef itk::ImageFileReader<RGBImageType> ImageFileReaderType;
  
  // Instantiate reader for input image.
  ImageFileReaderType::Pointer imageReader = ImageFileReaderType::New();
  imageReader->SetFileName(imageFileName);
  imageReader->Update();

  // Set input images from reader.
  RGBImageType::Pointer rgbImage = RGBImageType::New();
  rgbImage = imageReader->GetOutput();

  // Convert RGB image to HSV color space
  typedef itk::Accessor::RGBToHSVColorSpacePixelAccessor<unsigned char, float> RGBToHSVColorSpaceAccessorType;
  typedef itk::ImageAdaptor<RGBImageType, RGBToHSVColorSpaceAccessorType> RGBToHSVAdaptorType;
  RGBToHSVAdaptorType::Pointer rgbToHSVAdaptor = RGBToHSVAdaptorType::New();
  rgbToHSVAdaptor->SetImage(rgbImage);
    
  typedef itk::ImageFileWriter<RGBToHSVAdaptorType> OutputWriterType;
  OutputWriterType::Pointer writer = OutputWriterType::New();
  writer->SetInput(rgbToHSVAdaptor);
  writer->SetFileName(outputFileName);
  writer->Update();

  return EXIT_SUCCESS;
}

