#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkMinimumMaximumImageCalculator.h"
#include "itkAddConstantToImageFilter.h"

int main(int argc, char* argv[])
{
  std::string imageFileName = argv[1];
  std::string outputFileName = argv[2];

  typedef itk::Image<float, 2 >   ImageType;
  typedef itk::ImageFileReader<ImageType> ReaderType;

  ReaderType::Pointer imageReader = ReaderType::New();
  imageReader->SetFileName(imageFileName);
  imageReader->Update();

  typedef itk::MinimumMaximumImageCalculator <ImageType>
          ImageCalculatorFilterType;
   ImageCalculatorFilterType::Pointer imageCalculatorFilter
          = ImageCalculatorFilterType::New ();
  imageCalculatorFilter->SetImage(imageReader->GetOutput());
  imageCalculatorFilter->Compute();

  typedef itk::AddConstantToImageFilter <ImageType, float, ImageType> AddConstantToImageFilterType;
  AddConstantToImageFilterType::Pointer addConstantToImageFilter = AddConstantToImageFilterType::New();
  addConstantToImageFilter->SetInput(imageReader->GetOutput());
  addConstantToImageFilter->SetConstant(fabs(imageCalculatorFilter->GetMinimum()));
  addConstantToImageFilter->Update();

  typedef  itk::ImageFileWriter< ImageType > WriterType;
  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName(outputFileName);
  writer->SetInput(addConstantToImageFilter->GetOutput());
  writer->Update();

  return EXIT_SUCCESS;
}
