// ITK
#include "itkVectorImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkRegionOfInterestImageFilter.h"

// Submodules
#include "Helpers/Helpers.h"
#include "ITKHelpers/ITKHelpers.h"

std::vector<itk::Index<2> > GetBoundaryPixels(const itk::ImageRegion<2>& region, const unsigned int thickness)
{
  std::vector<itk::Index<2> > boundaryPixels;

  typedef itk::Image<float,2> DummyImageType;
  DummyImageType::Pointer dummyImage = DummyImageType::New();
  dummyImage->SetRegions(region);

  itk::ImageRegionIteratorWithIndex<DummyImageType> imageIterator(dummyImage, region);

  while(!imageIterator.IsAtEnd())
  {
    if( (abs(imageIterator.GetIndex()[0] - region.GetIndex()[0]) < static_cast<itk::Index<2>::IndexValueType>(thickness)) ||
        (abs(imageIterator.GetIndex()[0] - (region.GetIndex()[0] + region.GetSize()[0] - 1)) < static_cast<itk::Index<2>::IndexValueType>(thickness)) ||
        (abs(imageIterator.GetIndex()[1] - region.GetIndex()[1]) < static_cast<itk::Index<2>::IndexValueType>(thickness)) ||
        (abs(imageIterator.GetIndex()[1] - (region.GetIndex()[1] + region.GetSize()[1] - 1)) < static_cast<itk::Index<2>::IndexValueType>(thickness)))
    {
      boundaryPixels.push_back(imageIterator.GetIndex());
    }
    ++imageIterator;
  }

  return boundaryPixels;
}

//std::vector<itk::Index<2> > GetBoundaryPixels(const itk::ImageRegion<2>& region)
//{
//  std::vector<itk::Index<2> > boundaryPixels;

//  for(unsigned int i = region.GetIndex()[0]; i < region.GetIndex()[0] + region.GetSize()[0]; ++i)
//    {
//    itk::Index<2> index;
//    index[0] = i;
//    index[1] = region.GetIndex()[1];
//    boundaryPixels.push_back(index);

//    index[0] = i;
//    index[1] = region.GetIndex()[1] + region.GetSize()[1] - 1;
//    boundaryPixels.push_back(index);
//    }

//  for(unsigned int j = region.GetIndex()[1]; j < region.GetIndex()[1] + region.GetSize()[1]; ++j)
//    {
//    itk::Index<2> index;
//    index[0] = region.GetIndex()[0];
//    index[1] = j;
//    boundaryPixels.push_back(index);

//    index[0] = region.GetIndex()[0] + region.GetSize()[0] - 1;
//    index[1] = j;
//    boundaryPixels.push_back(index);
//    }

//  return boundaryPixels;
//}

template <typename TImage>
void SetPixels(TImage* const image, const std::vector<itk::Index<2> >& pixels, const typename TImage::PixelType& value)
{
  for(unsigned int i = 0; i < pixels.size(); ++i)
    {
    image->SetPixel(pixels[i], value);
    }
}

template<typename TImage>
void OutlineRegion(TImage* image, const itk::ImageRegion<2>& region, const typename TImage::PixelType& value, const unsigned int thickness)
{
  std::vector<itk::Index<2> > boundaryPixels = GetBoundaryPixels(region, thickness);

  SetPixels(image, boundaryPixels, value);
}

int main(int argc, char *argv[])
{
  // Verify arguments
  if(argc < 11)
    {
    std::cerr << "Required: inputFilename cornerX cornerY sizeX sizeY R G B thickness outputFilename" << std::endl;
    return EXIT_FAILURE;
    }

  // Output arguments
//  std::stringstream ss;
  std::cout << "Arguments:" << std::endl;
  for(int i = 1; i < argc; ++i)
  {
//    ss << argv[i] << " ";
    std::cout << argv[i] << " ";
  }
  std::cout << std::endl;

  typedef itk::Image<itk::CovariantVector<unsigned char>, 2> ImageType;

  // Parse arguments
  std::string inputFilename = argv[1];

  std::stringstream ssRegion;
  ssRegion << argv[2] << " " << argv[3] << " " << argv[4] << " " << argv[5];
  itk::Index<2> corner;
  itk::Size<2> size;
  ssRegion >> corner[0] >> corner[1] >> size[0] >> size[1];

  std::stringstream ssBoundaryValue;
  ssBoundaryValue << argv[6] << " " << argv[7] << " " << argv[8];
  // We have to parse into ints so that multiple character values (like 255), get read as "255" instead of "2", "5", "5"
  itk::CovariantVector<int> boundaryValueInt;
  ImageType::PixelType boundaryValue;
  ssBoundaryValue >> boundaryValueInt[0] >> boundaryValueInt[1] >> boundaryValueInt[2];
  boundaryValue[0] = boundaryValueInt[0];
  boundaryValue[1] = boundaryValueInt[1];
  boundaryValue[2] = boundaryValueInt[2];

  std::stringstream ssThickness;
  ssThickness << argv[9];
  int thickness;
  ssThickness >> thickness;

  std::string outputFilename = argv[10];

  ImageType::RegionType region(corner, size);

  // Output parsed arguments
  std::cout << "inputFilename " << inputFilename << std::endl;
  std::cout << "corner " << corner << std::endl;
  std::cout << "size " << size << std::endl;
  std::cout << "boundaryValue " << boundaryValue << std::endl;
  std::cout << "boundaryValue " << thickness << std::endl;
  std::cout << "outputFilename " << outputFilename << std::endl;

  // Read file

  typedef itk::ImageFileReader<ImageType> ReaderType;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(inputFilename);
  reader->Update();

  ImageType* image = reader->GetOutput();

  OutlineRegion(image, region, boundaryValue, thickness);

  // Write the result
  if(Helpers::GetFileExtension(outputFilename) == "png")
  {
    ITKHelpers::WriteRGBImage(image, outputFilename);
  }
  else
  {
    ITKHelpers::WriteImage(image, outputFilename);
  }

  return EXIT_SUCCESS;
}
