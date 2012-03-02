#include "itkImage.h"
#include "itkVectorImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"

typedef itk::VectorImage<float, 2> FloatImageType;
typedef itk::VectorImage<unsigned char, 2> UnsignedCharImageType;

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
  typedef itk::ImageFileReader<FloatImageType> ReaderType;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(inputFilename);
  reader->Update();

  UnsignedCharImageType::Pointer outputImage = UnsignedCharImageType::New();
  outputImage->SetNumberOfComponentsPerPixel(reader->GetOutput()->GetNumberOfComponentsPerPixel());
  outputImage->SetRegions(reader->GetOutput()->GetLargestPossibleRegion());
  outputImage->Allocate();
  
  itk::ImageRegionConstIterator<FloatImageType> inputIterator(reader->GetOutput(), reader->GetOutput()->GetLargestPossibleRegion());
  itk::ImageRegionIterator<UnsignedCharImageType> outputIterator(outputImage, outputImage->GetLargestPossibleRegion());

  while(!inputIterator.IsAtEnd())
    {
    outputIterator.Set(inputIterator.Get());
    ++inputIterator;
    ++outputIterator;
    }

  // Write the result
  typedef  itk::ImageFileWriter<UnsignedCharImageType> WriterType;
  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName(outputFilename);
  writer->SetInput(outputImage);
  writer->Update();

  return EXIT_SUCCESS;
}
