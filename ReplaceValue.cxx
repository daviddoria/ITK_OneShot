#include "itkVectorImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkImageRegionIterator.h"
#include "itkVariableLengthVector.h"

typedef itk::VectorImage<unsigned char, 2> VectorImageType;

// This program replaces the input value with the output value across all components of the input image
int main(int argc, char *argv[])
{
  // Verify arguments
  if(argc < 5)
    {
    std::cerr << "Required: inputFilename outputFilename inputValue outputValue" << std::endl;
    return EXIT_FAILURE;
    }

  // Parse arguments
  std::string inputFilename = argv[1];
  std::string outputFilename = argv[2];

  std::string strInputValue = argv[3];
  std::string strOutputValue = argv[4];


  std::stringstream ssInputValue;
  ssInputValue << strInputValue;
  float inputValue;
  ssInputValue >> inputValue;


  std::stringstream ssOutputValue;
  ssOutputValue << strOutputValue;
  float outputValue;
  ssOutputValue >> outputValue;

  // Display input parameters for confirmation
  std::cout << "inputFilename " << inputFilename << std::endl;
  std::cout << "outputFilename " << outputFilename << std::endl;
  std::cout << "Input value: " << inputValue << std::endl;
  std::cout << "Output value" << outputValue << std::endl;

  std::cout << std::endl;

  // Read the input file
  typedef itk::ImageFileReader<VectorImageType> ReaderType;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(inputFilename);
  reader->Update();

  // Create the output image
  VectorImageType::Pointer output = VectorImageType::New();
  output->SetRegions(reader->GetOutput()->GetLargestPossibleRegion());
  output->SetNumberOfComponentsPerPixel(reader->GetOutput()->GetNumberOfComponentsPerPixel());
  output->Allocate();

  itk::ImageRegionConstIterator<VectorImageType> inputIterator(reader->GetOutput(), reader->GetOutput()->GetLargestPossibleRegion());
  itk::ImageRegionIterator<VectorImageType> outputIterator(output, output->GetLargestPossibleRegion());
  typedef itk::VariableLengthVector<float> VariableVectorType;

  while(!inputIterator.IsAtEnd())
    {
    VariableVectorType pixel = inputIterator.Get();
    for(unsigned int i = 0; i < reader->GetOutput()->GetNumberOfComponentsPerPixel(); i++)
      {
      if(pixel[i] == inputValue)
        {
        pixel[i] = outputValue;
        }
      }
    outputIterator.Set(pixel);
    ++inputIterator;
    ++outputIterator;
    }

  typedef  itk::ImageFileWriter<VectorImageType> WriterType;
  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName(outputFilename);
  writer->SetInput(output);
  writer->Update();

  return EXIT_SUCCESS;
}
