#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkCovariantVector.h"
#include "itkComposeImageFilter.h"
#include "itkDerivativeImageFilter.h"

typedef itk::Image<float, 2> ImageType;
typedef itk::Image<itk::CovariantVector<float, 2>, 2> GradientImageType;

int main(int argc, char *argv[])
{
  if(argc < 3)
    {
    std::cerr << "Required: inputFilename outputFilename" << std::endl;
    return EXIT_FAILURE;
    }

  std::string inputFilename = argv[1];
  std::string outputFilename = argv[2];

  std::cout << "inputFilename " << inputFilename << std::endl;
  std::cout << "outputFilename " << outputFilename << std::endl;

  typedef itk::ImageFileReader<ImageType> ReaderType;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(inputFilename);
  reader->Update();

  typedef itk::DerivativeImageFilter<ImageType, ImageType> DerivativeImageFilterType;
  DerivativeImageFilterType::Pointer xDerivativeFilter = DerivativeImageFilterType::New();
  xDerivativeFilter->SetDirection(0);
  xDerivativeFilter->SetOrder(1);
  xDerivativeFilter->SetInput(reader->GetOutput());
  xDerivativeFilter->Update();

  DerivativeImageFilterType::Pointer yDerivativeFilter = DerivativeImageFilterType::New();
  yDerivativeFilter->SetOrder(1);
  yDerivativeFilter->SetDirection(1);
  yDerivativeFilter->SetInput(reader->GetOutput());
  yDerivativeFilter->Update();

  typedef itk::ComposeImageFilter<ImageType, GradientImageType> ComposeImageFilterType;
  ComposeImageFilterType::Pointer composeFilter = ComposeImageFilterType::New();
  composeFilter->SetInput1(xDerivativeFilter->GetOutput());
  composeFilter->SetInput2(yDerivativeFilter->GetOutput());
  composeFilter->Update();

  typedef  itk::ImageFileWriter<GradientImageType> WriterType;
  WriterType::Pointer xWriter = WriterType::New();
  xWriter->SetFileName(outputFilename);
  xWriter->SetInput(composeFilter->GetOutput());
  xWriter->Update();

  return EXIT_SUCCESS;
}
