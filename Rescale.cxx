#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkRescaleIntensityImageFilter.h"

typedef itk::Image<float, 2>  FloatImageType;

int main(int argc, char *argv[])
{
  // Verify arguments
  if(argc < 5)
    {
    std::cerr << "Required: inputFilename outputFilename low high" << std::endl;
    return EXIT_FAILURE;
    }

  // Parse arguments
  std::string inputFilename = argv[1];
  std::string outputFilename = argv[2];
  float low,high;
  std::stringstream ssLow;
  ssLow << argv[3];
  ssLow >> low;
  std::stringstream ssHigh;
  ssHigh << argv[4];
  ssHigh >> high;
  
  // Output arguments
  std::cout << "inputFilename " << inputFilename << std::endl;
  std::cout << "outputFilename " << outputFilename << std::endl;
  std::cout << "low " << low << std::endl;
  std::cout << "high " << high << std::endl;

  // Read file - the conversion from float to unsigned char is done during this step
  typedef itk::ImageFileReader<FloatImageType> ReaderType;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(inputFilename);
  reader->Update();

  typedef itk::RescaleIntensityImageFilter< FloatImageType, FloatImageType > RescaleFilterType;
  RescaleFilterType::Pointer rescaleFilter = RescaleFilterType::New();
  rescaleFilter->SetInput(reader->GetOutput());
  rescaleFilter->SetOutputMinimum(low);
  rescaleFilter->SetOutputMaximum(high);
  rescaleFilter->Update();

  // Write the result
  typedef  itk::ImageFileWriter<FloatImageType> WriterType;
  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName(outputFilename);
  writer->SetInput(rescaleFilter->GetOutput());
  writer->Update();

  return EXIT_SUCCESS;
}
