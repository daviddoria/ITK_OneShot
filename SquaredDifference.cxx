#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkSquaredDifferenceImageFilter.h"

typedef itk::Image<float, 2>  ImageType;

int main(int argc, char *argv[])
{
  // Verify arguments
  if(argc < 4)
    {
    std::cerr << "Required: input1Filename input2Filename outputFilename" << std::endl;
    return EXIT_FAILURE;
    }

  // Parse arguments
  std::string input1Filename = argv[1];
  std::string input2Filename = argv[2];
  std::string outputFilename = argv[3];

  // Output arguments
  std::cout << "input1Filename " << input1Filename << std::endl;
  std::cout << "input2Filename " << input2Filename << std::endl;
  std::cout << "outputFilename " << outputFilename << std::endl;

  // Read files
  typedef itk::ImageFileReader<ImageType> ReaderType;
  ReaderType::Pointer reader1 = ReaderType::New();
  reader1->SetFileName(input1Filename.c_str());
  reader1->Update();

  ReaderType::Pointer reader2 = ReaderType::New();
  reader2->SetFileName(input2Filename.c_str());
  reader2->Update();

  typedef itk::SquaredDifferenceImageFilter<ImageType, ImageType, ImageType> SquaredDifferenceImageFilterType;
  SquaredDifferenceImageFilterType::Pointer squaredDifferenceFilter = SquaredDifferenceImageFilterType::New();
  squaredDifferenceFilter->SetInput(0, reader1->GetOutput());
  squaredDifferenceFilter->SetInput(1, reader2->GetOutput());
  squaredDifferenceFilter->Update();

  typedef  itk::ImageFileWriter<ImageType> WriterType;
  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName(outputFilename);
  writer->SetInput(squaredDifferenceFilter->GetOutput());
  writer->Update();

  return EXIT_SUCCESS;
}
