#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkVectorImage.h"
#include "itkImageToVectorImageFilter.h"
#include "itkVectorIndexSelectionCastImageFilter.h"
#include "itkScaleTransform.h"
#include "itkResampleImageFilter.h"

typedef itk::Image<float, 2> ScalarImageType;
typedef itk::VectorImage<float, 2> VectorImageType;

void DeepCopy(const ScalarImageType::Pointer input, ScalarImageType::Pointer output);

int main(int argc, char *argv[])
{
  if(argc < 5)
    {
    std::cerr << "Required: inputFilename desiredWidth desiredHeight outputFilename" << std::endl;
    return EXIT_FAILURE;
    }

  std::string inputFilename = argv[1];
  
  unsigned int desiredWidth;
  unsigned int desiredHeight;
  
  std::stringstream ssWidth;
  ssWidth << argv[2];
  ssWidth >> desiredWidth;
  
  std::stringstream ssHeight;
  ssHeight << argv[3];
  ssHeight >> desiredHeight;
  
  std::string outputFilename = argv[4];

  std::cout << "inputFilename " << inputFilename << std::endl;
  std::cout << "desiredWidth " << desiredWidth << std::endl;
  std::cout << "desiredHeight " << desiredHeight << std::endl;
  std::cout << "outputFilename " << outputFilename << std::endl;

  typedef itk::ImageFileReader<VectorImageType> ReaderType;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(inputFilename);
  reader->Update();

  typedef itk::IdentityTransform<double, 2> T_Transform;
  
  typedef itk::ImageToVectorImageFilter<ScalarImageType> ImageToVectorImageFilterType;
  ImageToVectorImageFilterType::Pointer imageToVectorImageFilter = ImageToVectorImageFilterType::New();
    
  typedef itk::ResampleImageFilter<ScalarImageType, ScalarImageType> ResampleFilterType;
  ResampleFilterType::Pointer resampleFilter = ResampleFilterType::New();
  resampleFilter->SetTransform(T_Transform::New());
  
  float scale[2];
  scale[0] = static_cast<float>(desiredWidth) / static_cast<float>(reader->GetOutput()->GetLargestPossibleRegion().GetSize()[0]);
  scale[1] = static_cast<float>(desiredHeight) / static_cast<float>(reader->GetOutput()->GetLargestPossibleRegion().GetSize()[1]);
  std::cout << "Scale factors: " << scale[0] << " " << scale[1] << std::endl;
  
  itk::Size<2> newSize;
  newSize[0] = scale[0] * reader->GetOutput()->GetLargestPossibleRegion().GetSize()[0];
  newSize[1] = scale[1] * reader->GetOutput()->GetLargestPossibleRegion().GetSize()[1];
  
  std::cout << "New size: " << newSize << std::endl;
  
  ScalarImageType::SpacingType newSpacing;
  newSpacing[0] = reader->GetOutput()->GetSpacing()[0] * 1.0f/scale[0];
  newSpacing[1] = reader->GetOutput()->GetSpacing()[1] * 1.0f/scale[1];
  
  std::cout << "New spacing: " << newSize << std::endl;
  
  resampleFilter->SetSize(newSize);
  resampleFilter->SetOutputSpacing(newSpacing);
  
  std::vector<ScalarImageType::Pointer> strechedChannels;
  
  for(unsigned int component = 0; component < reader->GetOutput()->GetNumberOfComponentsPerPixel(); ++component)
    {
    typedef itk::VectorIndexSelectionCastImageFilter<VectorImageType, ScalarImageType> IndexSelectionType;
    IndexSelectionType::Pointer indexSelectionFilter = IndexSelectionType::New();
    indexSelectionFilter->SetIndex(component);
    indexSelectionFilter->SetInput(reader->GetOutput());
    indexSelectionFilter->Update();
  
    std::cout << "input size: " << indexSelectionFilter->GetOutput()->GetLargestPossibleRegion().GetSize() << std::endl;
  
    resampleFilter->SetInput(indexSelectionFilter->GetOutput());
    resampleFilter->Update();
  
    std::cout << "output size: " << resampleFilter->GetOutput()->GetLargestPossibleRegion().GetSize() << std::endl;
    
    strechedChannels.push_back(ScalarImageType::New());
    DeepCopy(resampleFilter->GetOutput(), strechedChannels[component]);
  
    imageToVectorImageFilter->SetNthInput(component, strechedChannels[component]);
  
    }

  imageToVectorImageFilter->Update();
  
  std::cout << "Final output size: " << imageToVectorImageFilter->GetOutput()->GetLargestPossibleRegion().GetSize() << std::endl;
  
  // Write the result
  typedef  itk::ImageFileWriter<VectorImageType> WriterType;
  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName(outputFilename);
  writer->SetInput(imageToVectorImageFilter->GetOutput());
  writer->Update();

  return EXIT_SUCCESS;
}

void DeepCopy(const ScalarImageType::Pointer input, ScalarImageType::Pointer output)
{
  output->SetRegions(input->GetLargestPossibleRegion());
  output->Allocate();
  itk::ImageRegionConstIterator<ScalarImageType> inputIterator(input, input->GetLargestPossibleRegion());
  itk::ImageRegionIterator<ScalarImageType> outputIterator(output, output->GetLargestPossibleRegion());

  while(!inputIterator.IsAtEnd())
    {
    outputIterator.Set(inputIterator.Get());
    ++inputIterator;
    ++outputIterator;
    }
}