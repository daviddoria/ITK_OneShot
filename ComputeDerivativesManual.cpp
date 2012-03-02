#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkCovariantVector.h"
#include "itkImageRegionIterator.h"
#include "itkNeighborhoodOperatorImageFilter.h"
#include "itkSobelOperator.h"
#include "itkDerivativeOperator.h"
#include "itkGaussianDerivativeOperator.h" // Requires ITK_USE_REVIEW

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

  // Setup operator
  //typedef itk::GaussianDerivativeOperator<float, 2> OperatorType;
  typedef itk::DerivativeOperator<float, 2> OperatorType;
  //typedef itk::SobelOperator<float, 2> OperatorType;
  itk::Size<2> radius;
  radius.Fill(1); // a radius of 1x1 creates a 3x3 operator

  // Compute both derivative images
  // X Derivative
  OperatorType operatorX;
  operatorX.SetDirection(0);
  operatorX.CreateToRadius(radius);
  operatorX.SetOrder(order);

  typedef itk::NeighborhoodOperatorImageFilter<ImageType, ImageType> NeighborhoodOperatorImageFilterType;
  NeighborhoodOperatorImageFilterType::Pointer xDerivativeFilter = NeighborhoodOperatorImageFilterType::New();
  xDerivativeFilter->SetOperator(operatorX);
  xDerivativeFilter->SetInput(reader->GetOutput());
  xDerivativeFilter->Update();

  // Y Derivative
  OperatorType operatorY;
  operatorY.SetDirection(1);
  operatorY.CreateToRadius(radius);
  operatorY.SetOrder(order);

  NeighborhoodOperatorImageFilterType::Pointer yDerivativeFilter = NeighborhoodOperatorImageFilterType::New();
  yDerivativeFilter->SetOperator(operatorY);
  yDerivativeFilter->SetInput(reader->GetOutput());
  yDerivativeFilter->Update();

//   if(argc == 4) // Write separate images for both derivates
//   {
    std::cout << "Writing separate images for both derivates." << std::endl;
    // Write the result
    typedef  itk::ImageFileWriter<ImageType> WriterType;

    WriterType::Pointer xWriter = WriterType::New();
    xWriter->SetFileName(outputFilenameX);
    xWriter->SetInput(xDerivativeFilter->GetOutput());
    xWriter->Update();

    WriterType::Pointer yWriter = WriterType::New();
    yWriter->SetFileName(outputFilenameY);
    yWriter->SetInput(yDerivativeFilter->GetOutput());
    yWriter->Update();
//   }
//   else
//   {
//     std::cout << "Combining derivatives into a 2 channel image." << std::endl;
//     // Combine the derivative images
//     typedef itk::Image<itk::CovariantVector<float, 2>, 2>  DerivativeImageType;
//     DerivativeImageType::Pointer derivativeImage = DerivativeImageType::New();
//     derivativeImage->SetRegions(reader->GetOutput()->GetLargestPossibleRegion());
//     derivativeImage->Allocate();
//
//     itk::ImageRegionIterator<DerivativeImageType> outputIterator(derivativeImage, derivativeImage->GetLargestPossibleRegion());
//     itk::ImageRegionConstIterator<ImageType> xDerivativeIterator(xDerivativeFilter->GetOutput(), xDerivativeFilter->GetOutput()->GetLargestPossibleRegion());
//     itk::ImageRegionConstIterator<ImageType> yDerivativeIterator(yDerivativeFilter->GetOutput(), yDerivativeFilter->GetOutput()->GetLargestPossibleRegion());
//
//     while(!outputIterator.IsAtEnd())
//       {
//       DerivativeImageType::PixelType pixel;
//       pixel[0] = xDerivativeIterator.Get();
//       pixel[1] = yDerivativeIterator.Get();
//
//       outputIterator.Set(pixel);
//       ++outputIterator;
//       ++xDerivativeIterator;
//       ++yDerivativeIterator;
//       }
//
//     // Write the result
//     typedef  itk::ImageFileWriter<DerivativeImageType> WriterType;
//     WriterType::Pointer writer = WriterType::New();
//     writer->SetFileName(outputFilename);
//     writer->SetInput(derivativeImage);
//     writer->Update();
//   }

  return EXIT_SUCCESS;
}
