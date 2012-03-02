#include <iostream>

#include "itkAddImageFilter.h"
#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkSquareImageFilter.h"

int main(int argc, char* argv[])
{
  // Verify arguments
  if(argc < 4)
    {
    std::cout << "Usage: SecondDerivativeX SecondDerivativeY outputLaplacian" << std::endl;
    exit(-1);
    }

  // Parse arguments
  std::string xSecondDerivativeImageFileName = argv[1];
  std::string ySecondDerivativeImageFileName = argv[2];
  std::string laplacianImageFilename = argv[3];

  // Output arguments
  std::cout << "xSecondDerivativeImageFileName " << xSecondDerivativeImageFileName << std::endl
            << "ySecondDerivativeImageFileName " << ySecondDerivativeImageFileName << std::endl
            << "laplacianImageFilename " << laplacianImageFilename << std::endl;

  typedef itk::Image<float, 2> FloatImageType;
  
  typedef itk::ImageFileReader<FloatImageType> ImageReaderType;
  ImageReaderType::Pointer xDerivativeImageReader = ImageReaderType::New();
  xDerivativeImageReader->SetFileName(xSecondDerivativeImageFileName);
  xDerivativeImageReader->Update();

  typedef itk::ImageFileReader<FloatImageType> ImageReaderType;
  ImageReaderType::Pointer yDerivativeImageReader = ImageReaderType::New();
  yDerivativeImageReader->SetFileName(ySecondDerivativeImageFileName);
  yDerivativeImageReader->Update();

  typedef itk::AddImageFilter<FloatImageType, FloatImageType> AddImageFilterType;

  AddImageFilterType::Pointer addFilter = AddImageFilterType::New ();
  addFilter->SetInput1(xDerivativeImageReader->GetOutput());
  addFilter->SetInput2(yDerivativeImageReader->GetOutput());
  addFilter->Update();
  
  typedef itk::ImageFileWriter<FloatImageType> ImageWriterType;
  ImageWriterType::Pointer writer = ImageWriterType::New();
  writer->SetFileName(laplacianImageFilename);
  writer->SetInput(addFilter->GetOutput());
  writer->Update();

  return EXIT_SUCCESS;
}
