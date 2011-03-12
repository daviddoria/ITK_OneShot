#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkAndImageFilter.h"

typedef itk::Image<unsigned char, 2>  RGBImageType;

int main(int argc, char *argv[])
{
  // Verify arguments
  if(argc < 4)
    {
    std::cerr << "Required: input1Filename input2Filename outputFilename" << std::endl;
    return EXIT_FAILURE;
    }

  // Parse arguments
  std::string input1Filename = argv[1];
  std::string input2Filename = argv[2];
  std::string outputFilename = argv[3];

  // Output arguments
  std::cout << "input1Filename " << input1Filename << std::endl;
  std::cout << "input2Filename " << input2Filename << std::endl;
  std::cout << "outputFilename " << outputFilename << std::endl;

  // Read files
  typedef itk::ImageFileReader<RGBImageType> ReaderType;
  ReaderType::Pointer reader1 = ReaderType::New();
  reader1->SetFileName(input1Filename.c_str());
  reader1->Update();

  ReaderType::Pointer reader2 = ReaderType::New();
  reader2->SetFileName(input2Filename.c_str());
  reader2->Update();

  typedef itk::AndImageFilter <RGBImageType> AndImageFilterType;
  AndImageFilterType::Pointer andFilter = AndImageFilterType::New ();
  andFilter->SetInput(0, reader1->GetOutput());
  andFilter->SetInput(1, reader2->GetOutput());
  andFilter->Update();

  typedef  itk::ImageFileWriter< RGBImageType  > WriterType;
  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName(outputFilename);
  writer->SetInput(andFilter->GetOutput());
  writer->Update();

  return EXIT_SUCCESS;
}
