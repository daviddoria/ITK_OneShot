#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkCovariantVector.h"
#include "itkImageRegionIterator.h"
#include "itkNeighborhoodOperatorImageFilter.h"
#include "itkForwardDifferenceOperator.h"
#include "itkComposeImageFilter.h"

typedef itk::Image<float, 2> ImageType;

int main(int argc, char *argv[])
{
  if(argc < 5)
    {
    std::cerr << "Required: inputFilename outputFilenameX outputFilenameY outputGradientFileName" << std::endl;
    return EXIT_FAILURE;
    }

  std::string inputFilename = argv[1];
  std::string outputFilenameX = argv[2];
  std::string outputFilenameY = argv[3];
  std::string outputFilenameGradient = argv[4];

  std::cout << "inputFilename " << inputFilename << std::endl;
  std::cout << "outputFilenameX " << outputFilenameX << std::endl;
  std::cout << "outputFilenameY " << outputFilenameY << std::endl;
  std::cout << "outputFilenameGradient " << outputFilenameGradient << std::endl;

  typedef itk::ImageFileReader<ImageType> ReaderType;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(inputFilename);
  reader->Update();

  // Setup operator
  typedef itk::ForwardDifferenceOperator<float, 2> OperatorType;
  itk::Size<2> radius;
  radius.Fill(1); // a radius of 1x1 creates a 3x3 operator

  // Compute both derivative images
  // X Derivative
  OperatorType operatorX;
  operatorX.SetDirection(0); // Create the operator for the X axis derivative
  operatorX.CreateToRadius(radius);

  typedef itk::NeighborhoodOperatorImageFilter<ImageType, ImageType> NeighborhoodOperatorImageFilterType;
  NeighborhoodOperatorImageFilterType::Pointer xDerivativeFilter = NeighborhoodOperatorImageFilterType::New();
  xDerivativeFilter->SetOperator(operatorX);
  xDerivativeFilter->SetInput(reader->GetOutput());
  xDerivativeFilter->Update();

  // Y Derivative
  OperatorType operatorY;
  operatorY.SetDirection(1); // Create the operator for the Y axis derivative
  operatorY.CreateToRadius(radius);

  NeighborhoodOperatorImageFilterType::Pointer yDerivativeFilter = NeighborhoodOperatorImageFilterType::New();
  yDerivativeFilter->SetOperator(operatorY);
  yDerivativeFilter->SetInput(reader->GetOutput());
  yDerivativeFilter->Update();

  // Write the resulting derivate images
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

  // Combine the derivative images
  typedef itk::ComposeImageFilter<ImageType> ComposeImageFilterType;
  ComposeImageFilterType::Pointer composeImageFilterType = ComposeImageFilterType::New();
  composeImageFilterType->SetInput(0, xDerivativeFilter->GetOutput());
  composeImageFilterType->SetInput(1, yDerivativeFilter->GetOutput());
  composeImageFilterType->Update();
  
  typedef  itk::ImageFileWriter<typename ComposeImageFilterType::OutputImageType> GradientWriterType;

  GradientWriterType::Pointer gradientWriter = GradientWriterType::New();
  gradientWriter->SetFileName(outputFilenameGradient);
  gradientWriter->SetInput(composeImageFilterType->GetOutput());
  gradientWriter->Update();

  return EXIT_SUCCESS;
}
