#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkThresholdImageFilter.h"

typedef itk::Image<float, 2> ImageType;

/**
 * Keep values inside the specified range, and set values outside the range to a valud.
 */

int main(int argc, char *argv[])
{
  // Verify arguments
  if(argc < 5)
    {
    std::cerr << "Required: inputFilename outputFilename thresholdLower thresholdUpper" << std::endl;
    return EXIT_FAILURE;
    }

  // Parse arguments
  std::string inputFilename = argv[1];
  std::string outputFilename = argv[2];
  

  std::stringstream ss;
  ss << argv[3] << " " << argv[4];
  float thresholdLower = 0.0f;
  float thresholdUpper = 0.0f;
  ss >> thresholdLower >> thresholdUpper;

  // Output arguments
  std::cout << "inputFilename " << inputFilename << std::endl;
  std::cout << "outputFilename " << outputFilename << std::endl;
  std::cout << "thresholdLower " << thresholdLower << std::endl;
  std::cout << "thresholdUpper " << thresholdUpper << std::endl;

  // Read file
  typedef itk::ImageFileReader<ImageType> ReaderType;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(inputFilename);
  reader->Update();

  // Threshold
  typedef itk::ThresholdImageFilter <ImageType> ThresholdImageFilterType;
  ThresholdImageFilterType::Pointer thresholdFilter = ThresholdImageFilterType::New();
  thresholdFilter->SetInput(reader->GetOutput());
  thresholdFilter->ThresholdOutside(thresholdLower, thresholdUpper);
  thresholdFilter->SetOutsideValue(0);
  thresholdFilter->Update();

  // Write the result
  typedef  itk::ImageFileWriter<ImageType> WriterType;
  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName(outputFilename);
  writer->SetInput(thresholdFilter->GetOutput());
  writer->Update();

  return EXIT_SUCCESS;
}
