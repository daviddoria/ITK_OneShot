#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkCannyEdgeDetectionImageFilter.h"

typedef itk::Image<float, 2> InputImageType;
typedef itk::Image<float, 2> OutputImageType;

int main(int argc, char *argv[])
{
  // Verify arguments
  if(argc < 4)
    {
    std::cerr << "Required: inputFilename outputFilename threshold" << std::endl;
    return EXIT_FAILURE;
    }

  // Parse arguments
  std::string inputFilename = argv[1];
  std::string outputFilename = argv[2];
  std::string strThreshold = argv[3];

  std::stringstream ss;
  ss << strThreshold;
  float threshold;
  ss >> threshold;

  // Output arguments
  std::cout << "inputFilename " << inputFilename << std::endl;
  std::cout << "outputFilename " << outputFilename << std::endl;
  std::cout << "thrshold " << threshold << std::endl;

  // Read file
  typedef itk::ImageFileReader<InputImageType> ReaderType;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(inputFilename);
  reader->Update();

  typedef itk::CannyEdgeDetectionImageFilter<InputImageType, OutputImageType>
    CannyEdgeDetectionImageFilterType;

  CannyEdgeDetectionImageFilterType::Pointer cannyFilter = CannyEdgeDetectionImageFilterType::New();
  cannyFilter->SetInput(reader->GetOutput());
  cannyFilter->SetVariance(1.0);
  //cannyFilter->SetUpperThreshold(upperThreshold);
  cannyFilter->SetLowerThreshold(threshold);
  cannyFilter->Update();

  // Write the result
  typedef itk::ImageFileWriter<OutputImageType> WriterType;
  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName(outputFilename);
  writer->SetInput(cannyFilter->GetOutput());
  writer->Update();

  return EXIT_SUCCESS;
}
