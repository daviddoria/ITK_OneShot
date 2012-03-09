#include "itkImage.h"
#include "itkVectorImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkCovariantVector.h"
#include "itkImageRegionIterator.h"
#include "itkVariableLengthVector.h"

typedef itk::Image<float, 2>  ScalarImageType;

typedef itk::VectorImage<float, 2> VectorImageType;

void SetComponent(VectorImageType::Pointer input, ScalarImageType::Pointer output, unsigned int component);

void DeepCopy(VectorImageType::Pointer input, VectorImageType::Pointer output);

// This program replaces 'component' component of inputFilename with replacementImageComponent and writes it to outputFilename
int main(int argc, char *argv[])
{
  // Verify arguments
  if(argc < 5)
    {
    std::cerr << "Required: inputFilename component replacementImageComponent outputFilename..." << std::endl;
    return EXIT_FAILURE;
    }

  // Parse arguments
  std::string inputFilename = argv[1];

  std::stringstream ss;
  ss << argv[2];
  int component;
  ss >> component;

  std::string replacementComponentFilename = argv[3];
  std::string outputFilename = argv[4];

  // Display input parameters for confirmation
  std::cout << "inputFilename " << inputFilename << std::endl;
  std::cout << "Component to extract: " << component << std::endl;
  std::cout << "replacementComponentFilename " << replacementComponentFilename << std::endl;
  std::cout << "outputFilename " << outputFilename << std::endl;

  std::cout << std::endl;

  // Read the input file
  typedef itk::ImageFileReader<VectorImageType> ReaderType;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(inputFilename);
  reader->Update();

  // Read the input file
  typedef itk::ImageFileReader<ScalarImageType> ScalarReaderType;
  ScalarReaderType::Pointer replacmentComponentReader = ScalarReaderType::New();
  replacmentComponentReader->SetFileName(replacementComponentFilename);
  replacmentComponentReader->Update();

  // Create the output image
  VectorImageType::Pointer output = VectorImageType::New();
  DeepCopy(reader->GetOutput(), output);

  SetComponent(output, replacmentComponentReader->GetOutput(), component);

  typedef  itk::ImageFileWriter<VectorImageType> WriterType;
  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName(outputFilename);
  writer->SetInput(output);
  writer->Update();

  return EXIT_SUCCESS;
}

void ExtractComponent(VectorImageType::Pointer input, ScalarImageType::Pointer output, unsigned int component)
{
  output->SetRegions(input->GetLargestPossibleRegion());
  output->Allocate();
  output->FillBuffer(0);

  itk::ImageRegionConstIterator<VectorImageType> inputIterator(input, input->GetLargestPossibleRegion());
  itk::ImageRegionIterator<ScalarImageType> outputIterator(output, output->GetLargestPossibleRegion());

  while(!inputIterator.IsAtEnd())
    {
    outputIterator.Set(inputIterator.Get()[component]);

    ++inputIterator;
    ++outputIterator;
    }
}

void SetComponent(VectorImageType::Pointer image, ScalarImageType::Pointer componentImage, unsigned int component)
{
  itk::ImageRegionIterator<VectorImageType> imageIterator(image, image->GetLargestPossibleRegion());
  itk::ImageRegionConstIterator<ScalarImageType> componentIterator(componentImage, componentImage->GetLargestPossibleRegion());

  while(!imageIterator.IsAtEnd())
    {
    itk::VariableLengthVector<float> pixel;
    pixel.SetSize(image->GetNumberOfComponentsPerPixel());
    pixel = imageIterator.Get();
    pixel[component] = componentIterator.Get();

    imageIterator.Set(pixel);

    ++imageIterator;
    ++componentIterator;
    }
}

void DeepCopy(VectorImageType::Pointer input, VectorImageType::Pointer output)
{
  output->SetRegions(input->GetLargestPossibleRegion());
  output->SetNumberOfComponentsPerPixel(input->GetNumberOfComponentsPerPixel());
  output->Allocate();

  itk::ImageRegionConstIterator<VectorImageType> inputIterator(input, input->GetLargestPossibleRegion());
  itk::ImageRegionIterator<VectorImageType> outputIterator(output, output->GetLargestPossibleRegion());

  while(!inputIterator.IsAtEnd())
    {
    outputIterator.Set(inputIterator.Get());
    ++inputIterator;
    ++outputIterator;
    }
}
