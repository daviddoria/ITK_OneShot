#include "itkVectorImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkRegionOfInterestImageFilter.h"

typedef itk::VectorImage<float, 2> ImageType;

int main(int argc, char *argv[])
{
  // Verify arguments
  if(argc < 7)
    {
    std::cerr << "Required: inputFilename cornerX cornerY sizeX sizeY outputFilename" << std::endl;
    return EXIT_FAILURE;
    }

  // Parse arguments
  std::string inputFilename = argv[1];
  std::stringstream ss;
  ss << argv[2] << " " << argv[3] << " " << argv[4] << " " << argv[5];
  itk::Index<2> corner;
  itk::Size<2> size;
  ss >> corner[0] >> corner[1] >> size[0] >> size[1];
  std::string outputFilename = argv[6];

  // Output arguments
  std::cout << "inputFilename " << inputFilename << std::endl;
  std::cout << "corner " << corner << std::endl;
  std::cout << "size " << size << std::endl;
  std::cout << "outputFilename " << outputFilename << std::endl;

  // Read file
  typedef itk::ImageFileReader<ImageType> ReaderType;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(inputFilename);
  reader->Update();

  typedef itk::RegionOfInterestImageFilter< ImageType, ImageType > FilterType;
  FilterType::Pointer filter = FilterType::New();
  ImageType::RegionType desiredRegion(corner, size);
  filter->SetRegionOfInterest(desiredRegion);
  filter->SetInput(reader->GetOutput());
  filter->Update();

  // Write the result
  typedef  itk::ImageFileWriter<ImageType> WriterType;
  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName(outputFilename);
  writer->SetInput(filter->GetOutput());
  writer->Update();

  return EXIT_SUCCESS;
}
