#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkDiscreteGaussianImageFilter.h"

typedef itk::Image<float, 2> ImageType;

int main(int argc, char *argv[])
{
  // Verify arguments
  if(argc < 4)
    {
    std::cerr << "Required: inputFilename outputFilename variance" << std::endl;
    return EXIT_FAILURE;
    }

  // Parse arguments
  std::string inputFilename = argv[1];
  std::string outputFilename = argv[2];
  //std::string strVariance = argv[3];
  std::stringstream ss;
  ss << argv[3];
  float variance;
  ss >> variance;
  
  // Output arguments
  std::cout << "inputFilename " << inputFilename << std::endl;
  std::cout << "outputFilename " << outputFilename << std::endl;
  std::cout << "variance: " << variance << std::endl;

  // Read file
  typedef itk::ImageFileReader<ImageType> ReaderType;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(inputFilename);
  reader->Update();

  typedef itk::DiscreteGaussianImageFilter<ImageType, ImageType >  DiscreteGaussianImageFilterType;
  DiscreteGaussianImageFilterType::Pointer discreteGaussianImageFilter = DiscreteGaussianImageFilterType::New();
  discreteGaussianImageFilter->SetInput( reader->GetOutput() );
  discreteGaussianImageFilter->SetVariance(variance);
  discreteGaussianImageFilter->Update();

  // Write the result
  typedef  itk::ImageFileWriter<ImageType> WriterType;
  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName(outputFilename);
  writer->SetInput(discreteGaussianImageFilter->GetOutput());
  writer->Update();

  return EXIT_SUCCESS;
}
