#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkOrImageFilter.h"
#include "itkRescaleIntensityImageFilter.h"

typedef itk::Image<unsigned char, 2>  ImageType;

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
  typedef itk::ImageFileReader<ImageType> ReaderType;
  ReaderType::Pointer reader1 = ReaderType::New();
  reader1->SetFileName(input1Filename);
  reader1->Update();

  {
  typedef itk::ImageFileWriter<ImageType> WriterType;
  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName("input1.png");
  writer->SetInput(reader1->GetOutput());
  writer->Update();
  }
  
  ReaderType::Pointer reader2 = ReaderType::New();
  reader2->SetFileName(input2Filename);
  reader2->Update();

  {
  typedef itk::ImageFileWriter<ImageType> WriterType;
  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName("input2.png");
  writer->SetInput(reader2->GetOutput());
  writer->Update();
  }
  
  typedef itk::OrImageFilter<ImageType> OrImageFilterType;
  OrImageFilterType::Pointer orFilter = OrImageFilterType::New();
  orFilter->SetInput1(reader1->GetOutput());
  orFilter->SetInput2(reader2->GetOutput());
  orFilter->Update();

  typedef itk::RescaleIntensityImageFilter<ImageType, ImageType> RescaleFilterType;
  RescaleFilterType::Pointer rescaleFilter = RescaleFilterType::New();
  rescaleFilter->SetInput(orFilter->GetOutput());
  rescaleFilter->SetOutputMinimum(0);
  rescaleFilter->SetOutputMaximum(255);
  rescaleFilter->Update();

  typedef itk::ImageFileWriter<ImageType> WriterType;
  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName(outputFilename);
  writer->SetInput(rescaleFilter->GetOutput());
  writer->Update();

  return EXIT_SUCCESS;
}
