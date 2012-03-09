#include "itkVectorImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkRegionOfInterestImageFilter.h"

typedef itk::VectorImage<float, 2> ImageType;

void DeepCopy(const ImageType* const input, ImageType* const output);

int main(int argc, char *argv[])
{
  // Verify arguments
  if(argc < 8)
    {
    std::cerr << "Required: copyFromFileName copyToFileName cornerX cornerY sizeX sizeY outputFileName" << std::endl;
    return EXIT_FAILURE;
    }

  // Parse arguments
  std::string copyFromFileName = argv[1];
  std::string copyToFileName = argv[2];
  std::stringstream ss;
  ss << argv[3] << " " << argv[4] << " " << argv[5] << " " << argv[6];
  itk::Index<2> corner;
  itk::Size<2> size;
  ss >> corner[0] >> corner[1] >> size[0] >> size[1];
  std::string outputFileName = argv[7];

  // Output arguments
  std::cout << "copyFromFileName " << copyFromFileName << std::endl;
  std::cout << "copyToFileName " << copyToFileName << std::endl;
  std::cout << "corner " << corner << std::endl;
  std::cout << "size " << size << std::endl;
  std::cout << "outputFilename " << outputFileName << std::endl;

  itk::ImageRegion<2> region(corner, size);
  
  // Read files
  typedef itk::ImageFileReader<ImageType> ReaderType;
  ReaderType::Pointer copyFromReader = ReaderType::New();
  copyFromReader->SetFileName(copyFromFileName);
  copyFromReader->Update();
  
  typedef itk::ImageFileReader<ImageType> ReaderType;
  ReaderType::Pointer copyToReader = ReaderType::New();
  copyToReader->SetFileName(copyToFileName);
  copyToReader->Update();

  ImageType::Pointer output = ImageType::New();
  DeepCopy(copyToReader->GetOutput(), output);
  
  itk::ImageRegionConstIterator<ImageType> copyFromIterator(copyFromReader->GetOutput(), region);
  itk::ImageRegionIterator<ImageType> outputIterator(output, region);

  while(!outputIterator.IsAtEnd())
    {
    outputIterator.Set(copyFromIterator.Get());

    ++outputIterator;
    ++copyFromIterator;
    }

  // Write the result
  typedef itk::ImageFileWriter<ImageType> WriterType;
  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName(outputFileName);
  writer->SetInput(output);
  writer->Update();

  return EXIT_SUCCESS;
}


void DeepCopy(const ImageType* const input, ImageType* const output)
{
  output->SetRegions(input->GetLargestPossibleRegion());
  output->SetNumberOfComponentsPerPixel(input->GetNumberOfComponentsPerPixel());
  output->Allocate();

  itk::ImageRegionConstIterator<ImageType> inputIterator(input, input->GetLargestPossibleRegion());
  itk::ImageRegionIterator<ImageType> outputIterator(output, output->GetLargestPossibleRegion());

  while(!inputIterator.IsAtEnd())
    {
    outputIterator.Set(inputIterator.Get());
    ++inputIterator;
    ++outputIterator;
    }
}
