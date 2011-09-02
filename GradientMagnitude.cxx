#include "itkGradientMagnitudeImageFilter.h"
#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"

typedef itk::Image<float, 2> ImageType;

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

  typedef itk::GradientMagnitudeImageFilter<ImageType, ImageType >  GradientMagnitudeImageFilterType;
  GradientMagnitudeImageFilterType::Pointer gradientMagnitudeImageFilter = GradientMagnitudeImageFilterType::New();
  gradientMagnitudeImageFilter->SetInput( reader->GetOutput() );
  gradientMagnitudeImageFilter->Update();

  // Write the result
  typedef  itk::ImageFileWriter<ImageType> WriterType;
  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName(outputFilename);
  writer->SetInput(gradientMagnitudeImageFilter->GetOutput());
  writer->Update();

  return EXIT_SUCCESS;
}
