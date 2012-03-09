#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkImageRegionIterator.h"
#include "itkDerivativeImageFilter.h"
#include "itkComposeImageFilter.h"

typedef itk::Image<float, 2> ImageType;

int main(int argc, char *argv[])
{
  if(argc < 6)
    {
    std::cerr << "Required: inputFilename outputFilenameX outputFilenameY outputGradientFileName order" << std::endl;
    return EXIT_FAILURE;
    }

  std::string inputFilename = argv[1];
  std::string outputFilenameX = argv[2];
  std::string outputFilenameY = argv[3];
  std::string outputFilenameGradient = argv[4];

  std::stringstream ssOrder;
  ssOrder << argv[5];
  unsigned int order = 1;
  ssOrder >> order;

  std::cout << "inputFilename " << inputFilename << std::endl;
  std::cout << "outputFilenameX " << outputFilenameX << std::endl;
  std::cout << "outputFilenameY " << outputFilenameY << std::endl;
  std::cout << "outputFilenameGradient " << outputFilenameGradient << std::endl;
  std::cout << "order " << order << std::endl;

  typedef itk::ImageFileReader<ImageType> ReaderType;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(inputFilename);
  reader->Update();

  typedef itk::DerivativeImageFilter<ImageType, ImageType> DerivativeImageFilterType;
  DerivativeImageFilterType::Pointer xDerivativeFilter = DerivativeImageFilterType::New();
  xDerivativeFilter->SetDirection(0);
  xDerivativeFilter->SetOrder(order);
  xDerivativeFilter->SetInput(reader->GetOutput());
  xDerivativeFilter->Update();

  DerivativeImageFilterType::Pointer yDerivativeFilter = DerivativeImageFilterType::New();
  yDerivativeFilter->SetOrder(order);
  yDerivativeFilter->SetDirection(1);
  yDerivativeFilter->SetInput(reader->GetOutput());
  yDerivativeFilter->Update();

  typedef itk::ComposeImageFilter<ImageType> ComposeImageFilterType;
  ComposeImageFilterType::Pointer composeImageFilterType = ComposeImageFilterType::New();
  composeImageFilterType->SetInput(0, xDerivativeFilter->GetOutput());
  composeImageFilterType->SetInput(1, yDerivativeFilter->GetOutput());
  composeImageFilterType->Update();

  {
  typedef  itk::ImageFileWriter<ImageType> WriterType;

  WriterType::Pointer xWriter = WriterType::New();
  xWriter->SetFileName(outputFilenameX);
  xWriter->SetInput(xDerivativeFilter->GetOutput());
  xWriter->Update();

  WriterType::Pointer yWriter = WriterType::New();
  yWriter->SetFileName(outputFilenameY);
  yWriter->SetInput(yDerivativeFilter->GetOutput());
  yWriter->Update();
  }

  typedef  itk::ImageFileWriter<typename ComposeImageFilterType::OutputImageType> GradientWriterType;

  GradientWriterType::Pointer gradientWriter = GradientWriterType::New();
  gradientWriter->SetFileName(outputFilenameGradient);
  gradientWriter->SetInput(composeImageFilterType->GetOutput());
  gradientWriter->Update();
  
  return EXIT_SUCCESS;
}
