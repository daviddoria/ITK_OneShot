#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkImageRegionConstIterator.h"

typedef itk::Image<unsigned char, 2> ImageType;

int main(int argc, char *argv[])
{
  // Verify arguments
  if(argc < 2)
    {
    std::cerr << "Required: inputFilename" << std::endl;
    return EXIT_FAILURE;
    }

  // Parse arguments
  std::string inputFilename = argv[1];

  // Output arguments
  std::cout << "inputFilename " << inputFilename << std::endl;

  // Read file
  typedef itk::ImageFileReader<ImageType> ReaderType;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(inputFilename);
  reader->Update();

  itk::ImageRegionConstIterator<ImageType> imageIterator(reader->GetOutput(), reader->GetOutput()->GetLargestPossibleRegion());

  itk::Index<2> min = {{reader->GetOutput()->GetLargestPossibleRegion().GetSize()[0], reader->GetOutput()->GetLargestPossibleRegion().GetSize()[1]}};
  itk::Index<2> max = {{0, 0}};
  
  while(!imageIterator.IsAtEnd())
    {
    if(imageIterator.Get())
    {
      if(imageIterator.GetIndex()[0] < min[0])
      {
        min[0] = imageIterator.GetIndex()[0];
      }
      if(imageIterator.GetIndex()[1] < min[1])
      {
        min[1] = imageIterator.GetIndex()[1];
      }
      if(imageIterator.GetIndex()[0] > max[0])
      {
        max[0] = imageIterator.GetIndex()[0];
      }
      if(imageIterator.GetIndex()[1] > max[1])
      {
        max[1] = imageIterator.GetIndex()[1];
      }
    }
    ++imageIterator;
    }

  itk::Size<2> size = {{max[0] - min[0], max[1] - min[1]}};
  itk::ImageRegion<2> region(min, size);
  std::cout << region << std::endl;
  
  return EXIT_SUCCESS;
}
