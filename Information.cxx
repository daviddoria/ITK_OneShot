#include "itkVectorImage.h"
#include "itkImageFileReader.h"

typedef itk::VectorImage<float, 2> ImageType;

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

  std::cout << "Number of components: " << reader->GetOutput()->GetNumberOfComponentsPerPixel() << std::endl;
  std::cout << "Region: " << reader->GetOutput()->GetLargestPossibleRegion() << std::endl;

  return EXIT_SUCCESS;
}
