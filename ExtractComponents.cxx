#include "itkImage.h"
#include "itkVectorImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkCovariantVector.h"
#include "itkImageRegionIterator.h"
#include "itkVariableLengthVector.h"

typedef itk::Image<float, 2>  ScalarImageType;

typedef itk::VectorImage<float, 2> VectorImageType;

void ExtractComponent(VectorImageType::Pointer input, ScalarImageType::Pointer output, unsigned int component);
void SetComponent(VectorImageType::Pointer input, ScalarImageType::Pointer output, unsigned int component);

int main(int argc, char *argv[])
{
  // Verify arguments
  if(argc < 4)
    {
    std::cerr << "Required: inputFilename outputFilename component component ..." << std::endl;
    return EXIT_FAILURE;
    }

  // Parse arguments
  std::string inputFilename = argv[1];
  std::string outputFilename = argv[2];
  std::vector<unsigned int> componentsToExtract;
  for(unsigned int i = 3; i < static_cast<unsigned int>(argc); i++)
    {
    std::stringstream ss;
    ss << argv[i];
    int component;
    ss >> component;
    componentsToExtract.push_back(component);
    }

  // Display input parameters for confirmation
  std::cout << "inputFilename " << inputFilename << std::endl;
  std::cout << "outputFilename " << outputFilename << std::endl;
  std::cout << "Components to extract: " << std::endl;
  for(unsigned int i = 0; i < componentsToExtract.size(); i++)
    {
    std::cout << componentsToExtract[i] << " ";
    }
  std::cout << std::endl;

  // Read the input file
  typedef itk::ImageFileReader<VectorImageType> ReaderType;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(inputFilename);
  reader->Update();

  // Create the output image
  VectorImageType::Pointer output = VectorImageType::New();
  output->SetRegions(reader->GetOutput()->GetLargestPossibleRegion());
  output->SetNumberOfComponentsPerPixel(componentsToExtract.size());
  output->Allocate();

  // Extract the requested components
  for(unsigned int i = 0; i < componentsToExtract.size(); i++)
    {
    ScalarImageType::Pointer outputComponent = ScalarImageType::New();
    outputComponent->SetRegions(reader->GetOutput()->GetLargestPossibleRegion());
    outputComponent->Allocate();
    ExtractComponent(reader->GetOutput(), outputComponent, componentsToExtract[i]);
    SetComponent(output, outputComponent, i);
    }

  typedef  itk::ImageFileWriter<VectorImageType> WriterType;
  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName(outputFilename);
  writer->SetInput(output);
  writer->Update();

  return EXIT_SUCCESS;
}

void ExtractComponent(VectorImageType::Pointer input, ScalarImageType::Pointer output, unsigned int component)
{
  if(component >= input->GetNumberOfComponentsPerPixel())
    {
    std::cerr << "Cannot extract component " << component << " from input with " << input->GetNumberOfComponentsPerPixel() << " components!" << std::endl;
    exit(-1);
    }
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
  if(component >= image->GetNumberOfComponentsPerPixel())
    {
    std::cerr << "Cannot set component " << component << " of image with " << image->GetNumberOfComponentsPerPixel() << " components!" << std::endl;
    exit(-1);
    }
    
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