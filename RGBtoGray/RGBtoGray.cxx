#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"

typedef itk::Image<unsigned char, 2>  GrayscaleImageType;

// Simply reading the file into a single channel image will do the conversion
// If you write it out as a png, it will still be 3 channel - you need to write it out as a mhd

int main(int argc, char *argv[])
{
  if(argc < 3)
    {
    std::cerr << "Required: inputFilename outputFilename" << std::endl;
    return EXIT_FAILURE;
    }

  std::string inputFilename = argv[1];
  std::string outputFilename = argv[2];

  typedef itk::ImageFileReader<GrayscaleImageType> ReaderType;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(inputFilename.c_str());
  reader->Update();

  typedef  itk::ImageFileWriter< GrayscaleImageType  > WriterType;
  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName(outputFilename);
  writer->SetInput(reader->GetOutput());
  writer->Update();

  return EXIT_SUCCESS;
}
