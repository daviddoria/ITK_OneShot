#include "itkImage.h"
//#include "itkVectorImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
//#include "itkRandomImageSource.h"
#include "itkMultiplyImageFilter.h"

typedef itk::Image< float, 2> ImageType;

int main(int argc, char *argv[])
{
  // Verify arguments
  if(argc < 4)
    {
    std::cerr << "Required: inputFilename scalar outputFilename" << std::endl;
    return EXIT_FAILURE;
    }

  // Parse arguments
  std::string inputFilename = argv[1];

  std::stringstream ss;
  ss << argv[2];
  float scalar;
  ss >> scalar;

  std::string outputFilename = argv[3];

  // Output arguments
  std::cout << "Input: " << inputFilename << std::endl;
  std::cout << "Scalar: " << scalar << std::endl;
  std::cout << "Output: " << outputFilename << std::endl;

  typedef itk::ImageFileReader<ImageType> ReaderType;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(inputFilename.c_str());
  reader->Update();

  /*
  ImageType::Pointer output = ImageType::New();
  output->SetRegions(reader->GetOutput()->GetLargestPossibleRegion());
  output->Allocate();

  
  itk::ImageRegionConstIterator<ImageType> inputIterator(reader->GetOutput(),
                                                         reader->GetOutput()->GetLargestPossibleRegion());
  itk::ImageRegionIterator<ImageType> outputIterator(output, output->GetLargestPossibleRegion());

  while(!outputIterator.IsAtEnd())
    {
    outputIterator.Set(inputIterator.Get()*scalar);

    ++outputIterator;
    ++inputIterator;
    }
  */
  
  typedef itk::MultiplyImageFilter<ImageType, ImageType, ImageType> MultiplyImageFilterType;
  MultiplyImageFilterType::Pointer multiplyImageFilter = MultiplyImageFilterType::New();
  multiplyImageFilter->SetInput(reader->GetOutput());
  multiplyImageFilter->SetConstant2(scalar);
  multiplyImageFilter->Update();
  
  typedef  itk::ImageFileWriter< ImageType  > WriterType;
  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName(outputFilename);
  writer->SetInput(multiplyImageFilter->GetOutput());
  writer->Update();

  return EXIT_SUCCESS;
}
