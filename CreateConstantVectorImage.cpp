#include "itkImage.h"
#include "itkImageFileWriter.h"

typedef itk::VectorImage<float, 2> ImageType;

template<typename TImage>
void SetRegionToConstant(TImage* const image, const itk::ImageRegion<2>& region,
                         const typename TImage::PixelType& constant);

int main(int argc, char *argv[])
{
  // Verify arguments
  if(argc < 6)
    {
    std::cerr << "Required: outputFilename sizeX sizeY components value" << std::endl;
    return EXIT_FAILURE;
    }

  // Parse arguments
  std::string outputFilename = argv[1];

  std::stringstream ss;
  ss << argv[2] << " " << argv[3] << " " << argv[4];

  itk::Size<2> size = {{0,0}};
  unsigned int components = 0;
  float value = 0.0f;
  ss >> size[0] >> size[1] >> components >> value;


  // Output arguments
  std::cout << "outputFilename " << outputFilename << std::endl;
  std::cout << "size " << size << std::endl;
  std::cout << "value " << value << std::endl;

  ImageType::Pointer image = ImageType::New();
  itk::Index<2> index = {{0,0}};

  itk::ImageRegion<2> region(index, size);

  image->SetRegions(region);
  image->SetNumberOfComponentsPerPixel(components);
  image->Allocate();

  typename itk::ImageRegionIterator<ImageType> imageIterator(image, image->GetLargestPossibleRegion());

  ImageType::PixelType pixel;
  pixel.SetSize(components);
  pixel.Fill(value);
  
  while(!imageIterator.IsAtEnd())
    {
    imageIterator.Set(pixel);

    ++imageIterator;
    }

  // Write the result
  typedef  itk::ImageFileWriter<ImageType> WriterType;
  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName(outputFilename);
  writer->SetInput(image);
  writer->Update();

  return EXIT_SUCCESS;
}
