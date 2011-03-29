#include "itkImage.h"
#include "itkVectorImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkBilateralImageFilter.h"

//typedef itk::VectorImage<float, 2> ImageType;
typedef itk::Image< float, 2> ImageType;

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

  typedef itk::ImageFileReader<ImageType> ReaderType;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(inputFilename.c_str());
  reader->Update();

  typedef itk::BilateralImageFilter<ImageType, ImageType> BilateralFilterType;
  BilateralFilterType::Pointer bilateralFilter = BilateralFilterType::New();
  bilateralFilter->SetInput(reader->GetOutput());
  float sigma = 1.0;
  bilateralFilter->SetDomainSigma(sigma);
  bilateralFilter->SetRangeSigma(sigma);
  bilateralFilter->Update();

  typedef  itk::ImageFileWriter<ImageType> WriterType;
  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName(outputFilename);
  writer->SetInput(bilateralFilter->GetOutput());
  writer->Update();

  return EXIT_SUCCESS;
}
