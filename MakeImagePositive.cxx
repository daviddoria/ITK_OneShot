#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkMinimumMaximumImageCalculator.h"
#include "itkAddImageFilter.h"

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

  typedef itk::AddImageFilter <ImageType> AddImageFilterType;
  AddImageFilterType::Pointer addImageFilter = AddImageFilterType::New();
  addImageFilter->SetInput1(imageReader->GetOutput());
  addImageFilter->SetConstant2(fabs(imageCalculatorFilter->GetMinimum()));
  addImageFilter->Update();

  typedef  itk::ImageFileWriter< ImageType > WriterType;
  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName(outputFileName);
  writer->SetInput(addImageFilter->GetOutput());
  writer->Update();

  return EXIT_SUCCESS;
}
