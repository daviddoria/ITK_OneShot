#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"

//typedef itk::Image< float, 2> ImageType;
typedef itk::Image<unsigned char, 2> ImageType;

int main(int argc, char *argv[])
{
  // Verify arguments
  if(argc < 3)
    {
    std::cerr << "Required: inputFilename outputFilename" << std::endl;
    return EXIT_FAILURE;
    }

  // Parse arguments
  std::string inputFilename = argv[1];
  std::string outputFilename = argv[2];

  typedef itk::ImageFileReader<ImageType> ReaderType;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(inputFilename.c_str());
  reader->Update();

  ImageType::Pointer output = ImageType::New();
  output->SetRegions(reader->GetOutput()->GetLargestPossibleRegion());
  output->Allocate();

  itk::ImageRegionConstIterator<ImageType> inputIterator(reader->GetOutput(),
                                                         reader->GetOutput()->GetLargestPossibleRegion());
  itk::ImageRegionIterator<ImageType> outputIterator(output, output->GetLargestPossibleRegion());

  while(!outputIterator.IsAtEnd())
    {
    if(inputIterator.Get())
      {
      outputIterator.Set(0);
      }
    else
      {
      outputIterator.Set(255);
      }

    ++outputIterator;
    ++inputIterator;
    }

  typedef  itk::ImageFileWriter<ImageType> WriterType;
  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName(outputFilename);
  writer->SetInput(output);
  writer->Update();

  return EXIT_SUCCESS;
}
