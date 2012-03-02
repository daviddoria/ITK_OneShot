#include "itkImage.h"
#include "itkImageFileWriter.h"

typedef itk::Image<float, 2> ImageType;

int main(int argc, char *argv[])
{
  // Verify arguments
  if(argc < 4)
    {
    std::cerr << "Required: outputFilename sizeX sizeY" << std::endl;
    return EXIT_FAILURE;
    }

  // Parse arguments
  std::string outputFilename = argv[1];

  std::stringstream ss;
  ss << argv[2] << " " << argv[3];
  itk::Size<2> size;
  ss >> size[0] >> size[1];
  
  // Output arguments
  std::cout << "outputFilename " << outputFilename << std::endl;

  ImageType::Pointer image = ImageType::New();
  itk::Index<2> index = {{0,0}};

  itk::ImageRegion<2> region(index,size);
  
  image->SetRegions(region);
  
  // Write the result
  typedef  itk::ImageFileWriter<ImageType> WriterType;
  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName(outputFilename);
  writer->SetInput(image);
  writer->Update();

  return EXIT_SUCCESS;
}
