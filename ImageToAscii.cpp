#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkImageRegionConstIterator.h"

#include <iostream>
#include <fstream>

typedef itk::Image<float, 2> ImageType;

int main(int argc, char *argv[])
{
  // Verify arguments
  if(argc < 3)
    {
    std::cerr << "Required: inputFilename outputFileName" << std::endl;
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

  itk::ImageRegionConstIterator<ImageType> imageIterator(reader->GetOutput(), reader->GetOutput()->GetLargestPossibleRegion());

  std::ofstream fout(outputFilename.c_str());

  unsigned int counter = 0;
  while(!imageIterator.IsAtEnd())
    {
    fout << imageIterator.Get() << " ";
    counter++;
    if(counter == reader->GetOutput()->GetLargestPossibleRegion().GetSize()[0])
    {
      counter = 0;
      fout << std::endl;
    }
    ++imageIterator;
    }

  fout.close();

  return EXIT_SUCCESS;
}
