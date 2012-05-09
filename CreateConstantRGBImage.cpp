#include "itkImage.h"
#include "itkImageFileWriter.h"

typedef itk::Image<itk::RGBPixel<unsigned char>, 2> ImageType;

template<typename TImage>
void SetRegionToConstant(TImage* const image, const itk::ImageRegion<2>& region,
                         const typename TImage::PixelType& constant);

int main(int argc, char *argv[])
{
  // Verify arguments
  if(argc < 7)
    {
    std::cerr << "Required: outputFilename sizeX sizeY R G B" << std::endl;
    return EXIT_FAILURE;
    }

  // Parse arguments
  std::string outputFilename = argv[1];

  std::stringstream ss;
  ss << argv[2] << " " << argv[3] << " " << argv[4] << " " << argv[5] << " " << argv[6];

  itk::Size<2> size = {{0,0}};
  int r,g,b;
  ss >> size[0] >> size[1] >> r >> g >> b;
  

  // Output arguments
  std::cout << "outputFilename " << outputFilename << std::endl;
  std::cout << "size " << size << std::endl;
  std::cout << "r " << r << std::endl;
  std::cout << "g " << g << std::endl;
  std::cout << "b " << b << std::endl;

  ImageType::Pointer image = ImageType::New();
  itk::Index<2> index = {{0,0}};

  itk::ImageRegion<2> region(index, size);
  
  image->SetRegions(region);
  image->Allocate();

  itk::RGBPixel<unsigned char> pixel;
  pixel.SetRed(r);
  pixel.SetGreen(g);
  pixel.SetBlue(b);

  SetRegionToConstant(image.GetPointer(), image->GetLargestPossibleRegion(), pixel);

  // Write the result
  typedef  itk::ImageFileWriter<ImageType> WriterType;
  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName(outputFilename);
  writer->SetInput(image);
  writer->Update();

  return EXIT_SUCCESS;
}

template<typename TImage>
void SetRegionToConstant(TImage* image, const itk::ImageRegion<2>& region, const typename TImage::PixelType& value)
{
  typename itk::ImageRegionIterator<TImage> imageIterator(image, region);

  while(!imageIterator.IsAtEnd())
    {
    imageIterator.Set(value);

    ++imageIterator;
    }
}
