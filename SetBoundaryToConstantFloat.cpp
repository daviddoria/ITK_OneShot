#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkImageRegionIteratorWithIndex.h"

typedef itk::Image<float, 2> ImageType;

int main(int argc, char *argv[])
{
  // Verify arguments
  if(argc < 4)
    {
    std::cerr << "Required: intputFilename constant outputFilename" << std::endl;
    return EXIT_FAILURE;
    }

  // Parse arguments
  std::string inputFilename = argv[1];

  std::stringstream ss;
  ss << argv[2];
  float constant = 0.0f;
  ss >> constant;

  std::string outputFilename = argv[3];
  
  // Output arguments
  std::cout << "inputFilename " << inputFilename << std::endl;
  std::cout << "constant: " << constant << std::endl;
  std::cout << "outputFilename " << outputFilename << std::endl;

  // Read the input
  typedef itk::ImageFileReader<ImageType> ReaderType;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(inputFilename);
  reader->Update();

  itk::ImageRegionIteratorWithIndex<ImageType> imageIterator(reader->GetOutput(), reader->GetOutput()->GetLargestPossibleRegion());

  while(!imageIterator.IsAtEnd())
    {
    if(imageIterator.GetIndex()[0] == 0 || static_cast<unsigned int>(imageIterator.GetIndex()[0]) == reader->GetOutput()->GetLargestPossibleRegion().GetSize()[0] - 1 ||
       imageIterator.GetIndex()[1] == 0 || static_cast<unsigned int>(imageIterator.GetIndex()[1]) == reader->GetOutput()->GetLargestPossibleRegion().GetSize()[1] - 1)
    {
      imageIterator.Set(constant);
    }
    ++imageIterator;
    }

  // Write the result
  typedef  itk::ImageFileWriter<ImageType> WriterType;
  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName(outputFilename);
  writer->SetInput(reader->GetOutput());
  writer->Update();

  return EXIT_SUCCESS;
}
