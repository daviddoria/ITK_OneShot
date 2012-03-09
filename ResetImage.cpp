#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkFixedArray.h"

typedef itk::VectorImage<float, 2>  ImageType;

int main(int argc, char *argv[])
{
  if(argc < 3)
    {
    std::cerr << "Required: inputFilename outputFilename" << std::endl;
    return EXIT_FAILURE;
    }

  std::string inputFilename = argv[1];
  std::string outputFilename = argv[2];

  typedef itk::ImageFileReader<ImageType> ReaderType;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(inputFilename.c_str());
  reader->Update();

  ImageType::Pointer output = ImageType::New();
  output->SetNumberOfComponentsPerPixel(reader->GetOutput()->GetNumberOfComponentsPerPixel());
  output->SetRegions(reader->GetOutput()->GetLargestPossibleRegion());
  output->Allocate();
  
  itk::ImageRegionIterator<ImageType> outputIterator(output, output->GetLargestPossibleRegion());

  while(!outputIterator.IsAtEnd())
    {
    outputIterator.Set(reader->GetOutput()->GetPixel(outputIterator.GetIndex()));

    ++outputIterator;
    }

  typedef  itk::ImageFileWriter<ImageType> WriterType;
  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName(outputFilename);
  writer->SetInput(output);
  writer->Update();

  return EXIT_SUCCESS;
}
