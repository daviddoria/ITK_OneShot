#include "itkImage.h"
#include "itkVectorImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkRandomImageSource.h"

typedef itk::Image<unsigned char, 2> ImageType;
void DeepCopy(ImageType::Pointer input, ImageType::Pointer output);

int main(int argc, char *argv[])
{
  // Verify arguments
  if(argc < 4)
    {
    std::cerr << "Required: inputFilename outputFilename constant" << std::endl;
    return EXIT_FAILURE;
    }

  // Parse arguments
  std::string inputFilename = argv[1];
  std::string outputFilename = argv[2];
  std::string strConstant = argv[3];

  std::stringstream ss;
  ss << strConstant;
  unsigned char constant;
  ss >> constant;

  std::cout << "Constant: " << constant << std::endl;

  typedef itk::ImageFileReader<ImageType> ReaderType;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(inputFilename.c_str());
  reader->Update();

  ImageType::Pointer output = ImageType::New();

  DeepCopy(reader->GetOutput(), output);

  // Set boundary pixels to the specified constant
  itk::ImageRegionIterator<ImageType> outputIterator(output, output->GetLargestPossibleRegion());

  while(!outputIterator.IsAtEnd())
    {
    itk::Index<2> index = outputIterator.GetIndex();
    if(index[0] == 0 || index[0] == output->GetLargestPossibleRegion().GetSize()[0] -1 ||
       index[1] == 0 || index[1] == output->GetLargestPossibleRegion().GetSize()[1] -1)
      {
      outputIterator.Set(constant);
      }

    ++outputIterator;
    }

  typedef  itk::ImageFileWriter<ImageType> WriterType;
  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName(outputFilename);
  writer->SetInput(output);
  writer->Update();

  return EXIT_SUCCESS;
}


void DeepCopy(ImageType::Pointer input, ImageType::Pointer output)
{
  output->SetRegions(input->GetLargestPossibleRegion());
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