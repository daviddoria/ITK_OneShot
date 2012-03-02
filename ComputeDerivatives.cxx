#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkCovariantVector.h"
#include "itkImageRegionIterator.h"
#include "itkNeighborhoodOperatorImageFilter.h"
#include "itkDerivativeImageFilter.h"

typedef itk::Image<float, 2> ImageType;

int main(int argc, char *argv[])
{
  if(argc < 4)
    {
    std::cerr << "Required: inputFilename outputFilenameX outputFilenameY order" << std::endl;
    return EXIT_FAILURE;
    }

  std::string inputFilename = argv[1];
  std::string outputFilenameX = argv[2];
  std::string outputFilenameY = argv[3];

  std::stringstream ssOrder;
  ssOrder << argv[4];
  unsigned int order = 1;
  ssOrder >> order;

  std::cout << "inputFilename " << inputFilename << std::endl;
  std::cout << "outputFilenameX " << outputFilenameX << std::endl;
  std::cout << "outputFilenameY " << outputFilenameY << std::endl;
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

  typedef  itk::ImageFileWriter<ImageType> WriterType;

  WriterType::Pointer xWriter = WriterType::New();
  xWriter->SetFileName(outputFilenameX);
  xWriter->SetInput(xDerivativeFilter->GetOutput());
  xWriter->Update();

  WriterType::Pointer yWriter = WriterType::New();
  yWriter->SetFileName(outputFilenameY);
  yWriter->SetInput(yDerivativeFilter->GetOutput());
  yWriter->Update();

  return EXIT_SUCCESS;
}
